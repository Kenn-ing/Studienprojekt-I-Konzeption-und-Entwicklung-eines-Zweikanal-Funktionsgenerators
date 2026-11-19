/**
 * @file uart_receiver.cpp
 * @brief Der ESP32 Dev Modul ist der Empfänger der UART-Kommunikation zwischen den Mikrocontrollern.
 * Erhält die Gruppen vom ESP32S3 und speichert diese im NVS.
 */
//----------------------------------------------------------------------
#include "uart.h"
#include "save_data.h"
#include "dds_output.h"

// Zustandsvariable: false = wartet auf COUNT, true = empfängt gerade Gruppen
static bool    receiving       = false;
static uint8_t groups_expected = 0;    // Anzahl der erwarteten Gruppen laut COUNT
static uint8_t group_count     = 0;    // Anzahl der bisher empfangenen Gruppen
static SignalGroup puffer[MAX_GROUPS]; // Temporärer Speicher für die empfangenen Gruppen

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
 * Erwartet das Format: G:%u,A1:%f,F1:%f,P1:%f,S1:%hhu,A2:%f,F2:%f,P2:%f,S2:%hhu
 * Gibt true zurück wenn alle 9 Felder erfolgreich gelesen wurden.
 */
static bool parse_group(const String& line, SignalGroup& out) {
    unsigned int id;
    float a1, f1, p1, a2, f2, p2;
    uint8_t s1, s2;

    // Alle 9 Felder aus dem String lesen
    int scanned = sscanf(line.c_str(),
        "G:%u,A1:%f,F1:%f,P1:%f,S1:%hhu,A2:%f,F2:%f,P2:%f,S2:%hhu",
        &id, &a1, &f1, &p1, &s1, &a2, &f2, &p2, &s2);

    if (scanned != 9) {
        Serial.printf("Parse-Fehler: nur %d/9 Felder (%s)\n", scanned, line.c_str());
        return false;
    }

    // Gelesene Werte in die SignalGroup schreiben
    out.group_id         = (uint8_t)id;
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
 * COUNT -> Anzahl der erwarteten Gruppen
 * G:... -> eine Gruppenzeile parsen und speichern
 * DONE  -> alle Gruppen im NVS speichern
 * START -> Gruppe laden und DAC starten
 * STOP  -> DAC stoppen
 */
void uart_loop() {
    if (!UART_PORT.available()) return;  // Nichts empfangen -> nichts tun

    String line = UART_PORT.readStringUntil('\n');  // Eine Zeile lesen
    line.trim();                                     // Leerzeichen und \r entfernen
    if (line.isEmpty()) return;                      // Leere Zeile ignorieren

    Serial.printf("UART RX: %s\n", line.c_str());   // Debug-Ausgabe

    // COUNT:<n> 
    if (line.startsWith("COUNT:")) {
        groups_expected = (uint8_t)line.substring(6).toInt();  // Anzahl der erwarteten Gruppen lesen
        group_count     = 0;                                    // Zähler zurücksetzen
        receiving       = true;                                 // Empfang starten
        Serial.printf("Empfange %u Gruppen...\n", groups_expected);

    // G:... 
    } else if (receiving && line.startsWith("G:")) {
        SignalGroup g;
        bool success = parse_group(line, g);  // Zeile parsen, Ergebnis speichern

        if (success == false) {
            UART_PORT.println("NACK");        // Fehler -> NACK senden
            return;
        }
        puffer[group_count++] = g;            // Gruppe im Puffer speichern
        UART_PORT.println("ACK");             // Bestätigung senden

    // DONE 
    } else if (line == "DONE") {
        if (group_count != groups_expected) {
            Serial.printf("Warnung: %u erwartet, %u empfangen\n",
                          groups_expected, group_count);
        }
        nvs_save_all(puffer, group_count);          // Alle Gruppen im NVS speichern
        receiving = false;                                 // Empfang beenden
        Serial.printf("%u Gruppen gespeichert.\n", group_count);

    // START:<id> 
    } else if (line.startsWith("START:")) {
        uint8_t id = (uint8_t)line.substring(6).toInt();  // Gruppen-ID lesen
        SignalGroup g;
        bool success = nvs_load_group(id, g);             // Gruppe aus NVS laden

        if (success == false) {
            UART_PORT.println("NACK");
            Serial.printf("Gruppe %u nicht im NVS\n", id);
            return;
        }
        UART_PORT.println("ACK");
        Serial.printf("Starte Gruppe %u\n", id);
        dds_start(&g, 1);                         // DDS Ausgabe starten

    // STOP 
    } else if (line == "STOP") {
        UART_PORT.println("ACK");
        dds_stop();                              // DDS Ausgabe stoppen
        Serial.println("DDS gestoppt");
    }
}