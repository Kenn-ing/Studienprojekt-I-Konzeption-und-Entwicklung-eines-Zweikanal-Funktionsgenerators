/**
 * @file uart_receiver.cpp
 * @brief Der ESP32 Dev Modul ist der Empfänger der UART-Kommunikation zwischen den Mikrocontrollern.
 * Erhält die Gruppen vom ESP32S3 und speichert diese im NVS.
 */
//----------------------------------------------------------------------
#include "uart.h"
#include "save_data.h"
#include "dds_output.h"

// Zustandsvariable: false = wartet auf CFG, true = empfängt gerade Gruppen
static bool    receiving       = false;
static uint32_t groups_expected = 0;      // Anzahl der erwarteten Gruppen laut CFG (M*T*S aus der CSV)
static uint32_t group_count     = 0;      // Anzahl der bisher empfangenen Gruppen
static SignalGroup* puffer      = nullptr; // Temporärer Speicher für die empfangenen Gruppen, wird nach CFG in der passenden Größe angelegt
static GroupConfig rx_cfg = {0, 0, 0};    // Anzahlen der Übertragung, die gerade läuft
static GroupConfig cfg    = {0, 0, 0};    // Anzahlen der Gruppen, die aktuell im NVS liegen (damit prüfen wir START)
static bool cfg_loaded    = false;        // true sobald cfg gültig ist (nach erfolgreichem DONE oder aus dem NVS geladen)

/**
 * Gibt den Zwischenspeicher wieder frei.
 * Wird nach DONE (egal ob erfolgreich oder nicht) und vor einer neuen Übertragung aufgerufen.
 */
static void free_puffer() {
    free(puffer);
    puffer = nullptr;
}

/**
 * Initialisiert die UART Kommunikation
 * Baudrate, Konstante (SERIAL_8N1: 8 Datenbits, No Parity und 1 Stoppbit), RX-Pin und TX-Pin
 */
bool uart_init() {
    UART_PORT.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
    Serial.println("UART bereit");
    return true;
}

/**
 * Parst eine empfangene Gruppenzeile und befüllt eine SignalGroup.
 * Erwartet das Format: G:%u,T:%u,U:%u,A1:%f,F1:%f,P1:%f,S1:%hhu,A2:%f,F2:%f,P2:%f,S2:%hhu
 * (Hauptgruppe, Aufgabe, Untergruppe und danach die Parameter von Kanal 1 und 2)
 * Gibt true zurück wenn alle 11 Felder erfolgreich gelesen wurden und die Gruppe zu den Anzahlen aus CFG passt.
 */
static bool parse_group(const String& line, SignalGroup& out) {
    unsigned int m, t, u;
    float a1, f1, p1, a2, f2, p2;
    uint8_t s1, s2;

    // Alle 11 Felder aus dem String lesen
    int scanned = sscanf(line.c_str(),
        "G:%u,T:%u,U:%u,A1:%f,F1:%f,P1:%f,S1:%hhu,A2:%f,F2:%f,P2:%f,S2:%hhu",
        &m, &t, &u, &a1, &f1, &p1, &s1, &a2, &f2, &p2, &s2);

    if (scanned != 11) {
        Serial.printf("Parse-Fehler: nur %d/11 Felder (%s)\n", scanned, line.c_str());
        return false;
    }

    // Liegt die Gruppe innerhalb der Anzahlen aus CFG? (vor dem Umwandeln auf uint8_t prüfen, sonst wird z.B. aus 256 eine 0)
    if (m < 1 || m > rx_cfg.mainGroups || t < 1 || t > rx_cfg.tasks || u < 1 || u > rx_cfg.subGroups) {
        Serial.printf("Parse-Fehler: Gruppe H%u A%u U%u gibt es nicht\n", m, t, u);
        return false;
    }

    // Gelesene Werte in die SignalGroup schreiben
    out.main_group       = (uint8_t)m;
    out.task             = (uint8_t)t;
    out.sub_group        = (uint8_t)u;
    out.ch1.amplitude_mV = a1;
    out.ch1.frequency_Hz = f1;
    out.ch1.phase_deg    = p1;
    out.ch1.signalType   = s1;
    out.ch2.amplitude_mV = a2;
    out.ch2.frequency_Hz = f2;
    out.ch2.phase_deg    = p2;
    out.ch2.signalType   = s2;

    return true;
}

/**
 * Hauptschleife der UART-Kommunikation — wird kontinuierlich in loop() aufgerufen.
 * Liest eingehende Nachrichten und reagiert entsprechend dem Protokoll:
 * CFG -> Anzahlen (Hauptgruppen, Aufgaben, Untergruppen) und Gesamtzahl der erwarteten Gruppen
 * G:... -> eine Gruppenzeile parsen und speichern
 * DONE -> prüfen ob alle Gruppen da sind und im NVS speichern
 * START -> Gruppe laden und DAC starten
 * STOP  -> DAC stoppen
 */
void uart_loop() {
    if (!UART_PORT.available()) return;  // Nichts empfangen -> nichts tun

    String line = UART_PORT.readStringUntil('\n');  // Eine Zeile lesen
    line.trim();                                     // Leerzeichen und \r entfernen
    if (line.isEmpty()) return;                      // Leere Zeile ignorieren

    Serial.printf("UART RX: %s\n", line.c_str());   // Debug-Ausgabe

    // CFG:<M>,<T>,<S>,<N>
    if (line.startsWith("CFG:")) {
        unsigned int m, t, s;
        unsigned long n;
        int scanned = sscanf(line.c_str(), "CFG:%u,%u,%u,%lu", &m, &t, &s, &n);

        // Alle 4 Werte gelesen, jeder Wert passt in ein Byte und N stimmt mit M*T*S überein?
        GroupConfig c = { (uint8_t)m, (uint8_t)t, (uint8_t)s };
        if (scanned != 4 || m > 255 || t > 255 || s > 255 || !config_valid(c) || group_total(c) != n) {
            UART_PORT.println("NACK");
            Serial.println("CFG ungueltig");
            return;
        }

        // Zwischenspeicher in genau der Größe anlegen, die laut CSV gebraucht wird (calloc setzt alles auf 0)
        free_puffer();
        puffer = (SignalGroup*)calloc(n, sizeof(SignalGroup));
        if (!puffer) {
            receiving = false;
            UART_PORT.println("NACK");
            Serial.println("Nicht genug Speicher fuer die Gruppen");
            return;
        }

        rx_cfg          = c;     // gilt für die Übertragung, die jetzt startet
        groups_expected = n;     // Anzahl der erwarteten Gruppen
        group_count     = 0;     // Zähler zurücksetzen
        receiving       = true;  // Empfang starten
        UART_PORT.println("ACK");
        Serial.printf("Empfange %lu Gruppen (%u Hauptgruppen, %u Aufgaben, %u Untergruppen)...\n",
                      n, c.mainGroups, c.tasks, c.subGroups);

    // G:... 
    } else if (receiving && line.startsWith("G:")) {
        SignalGroup g;
        bool success = parse_group(line, g);  // Zeile parsen, Ergebnis speichern

        if (success == false) {
            UART_PORT.println("NACK");        // Fehler -> NACK senden
            return;
        }
        SignalGroup& slot = puffer[group_index(rx_cfg, g.main_group, g.task, g.sub_group)];
        if (slot.main_group == 0) group_count++;  // Platz war noch leer -> neue Gruppe (doppelte Zeilen zählen nicht doppelt)
        slot = g;                                 // Gruppe im Puffer speichern
        UART_PORT.println("ACK");             // Bestätigung senden

    // DONE 
    } else if (line == "DONE") {
        // Alle erwarteten Gruppen angekommen? Sonst nichts speichern, das HMI zeigt dann den Fehler an
        if (!receiving || group_count != groups_expected) {
            Serial.printf("Fehler: %lu erwartet, %lu empfangen\n",
                          (unsigned long)groups_expected, (unsigned long)group_count);
            UART_PORT.println("NACK");
            receiving = false;
            free_puffer();
            return;
        }

        bool saved = nvs_save_all(puffer, rx_cfg, group_count);  // Alle Gruppen im NVS speichern
        receiving = false;                                       // Empfang beenden
        free_puffer();                                           // Zwischenspeicher wird nicht mehr gebraucht (START lädt aus dem NVS)

        if (saved == false) {
            UART_PORT.println("NACK");
            Serial.println("Speichern im NVS fehlgeschlagen");
            cfg_loaded = false;                                  // NVS-Stand ist nicht mehr verlässlich
            return;
        }

        cfg = rx_cfg;                                            // neue Anzahlen gelten ab jetzt für START
        cfg_loaded = true;
        UART_PORT.println("ACK");                                // erst jetzt bestätigen, damit das HMI weiß: alles gespeichert
        Serial.printf("%lu Gruppen gespeichert.\n", (unsigned long)group_count);

    // START:<m>,<t>,<s>
    } else if (line.startsWith("START:")) {
        unsigned int m, t, s;
        if (sscanf(line.c_str(), "START:%u,%u,%u", &m, &t, &s) != 3) {
            UART_PORT.println("NACK");
            return;
        }

        // Anzahlen aus dem NVS holen falls der Slave neu gestartet wurde und noch nichts empfangen hat
        if (!cfg_loaded) cfg_loaded = nvs_load_config(cfg);

        SignalGroup g;
        bool success = cfg_loaded && m <= 255 && t <= 255 && s <= 255 &&
                       nvs_load_group(cfg, (uint8_t)m, (uint8_t)t, (uint8_t)s, g);  // Gruppe aus NVS laden

        if (success == false) {
            UART_PORT.println("NACK");
            Serial.printf("Gruppe H%u A%u U%u nicht im NVS\n", m, t, s);
            return;
        }
        UART_PORT.println("ACK");
        Serial.printf("Starte Gruppe H%u A%u U%u\n", m, t, s);
        dds_start(&g, 1);                         // DDS Ausgabe starten

    // STOP 
    } else if (line == "STOP") {
        UART_PORT.println("ACK");
        dds_stop();                              // DDS Ausgabe stoppen
        Serial.println("DDS gestoppt");
    }
}