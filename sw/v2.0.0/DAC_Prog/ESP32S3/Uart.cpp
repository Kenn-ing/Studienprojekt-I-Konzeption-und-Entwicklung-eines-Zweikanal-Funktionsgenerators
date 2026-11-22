/**
 * @file Uart.cpp
 * @brief Umsetzung der UART Kommunikation
 *
 * Protokoll (jede Nachricht endet mit '\n', der Slave antwortet jeweils mit "ACK" oder "NACK"):
 *   CFG:M,T,S,N                      Anzahlen + Gesamtzahl N = M*T*S        -> ACK
 *   G:m,T:t,U:s,A1:..,F1:..,...      eine Gruppe (nach CFG, N mal)          -> ACK
 *   DONE                             Ende; Slave prüft Vollständigkeit,
 *                                    speichert im NVS                       -> ACK / NACK
 *   START:m,t,s                      Gruppe starten                         -> ACK
 *   STOP                             Ausgabe stoppen                        -> ACK
 */
//----------------------------------------------------------------------
#include "Uart.h"

/**
* Initialisieren der UART funktion
* Baudrate, Konstante (Serial_8N1: 8 Datenbits, No Parity und 1 Stoppbit), Rx-Pin und Tx-Pin
*/ 
bool uart_init() {
    UART_PORT.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
    delay(100);
    return true;
}

/**
* Wartet auf "ACK" oder "NACK" vom Empfänger.
* Liefert true bei "ACK", false bei "NACK" oder wenn innerhalb von timeout_ms nichts kommt.
* Der Timeout ist einstellbar, weil der Slave nach "DONE" erst ins NVS schreibt (dauert länger).
*/
static bool ACK_wait(uint32_t timeout_ms = UART_ACK_TIMEOUT_MS) {
    unsigned long t = millis();
    while (millis() - t < timeout_ms) {
        if (UART_PORT.available()) {                            // Prüft ob Daten verfügbar sind
            String answer = UART_PORT.readStringUntil('\n');    // Lesen bis zum Zeilenende
            answer.trim();                                      // entfernt Leerzeichen
            if (answer == "ACK")  return true;
            if (answer == "NACK") return false;
        }
    }
    Serial.println("UART: Timeout, kein ACK erhalten");
    return false;
}

/**
* Verpackt eine Gruppe in eine Textzeile und überträgt sie an das ESP32 Dev Modul.
* Format: G:<m>,T:<t>,U:<s>,A1:..,F1:..,P1:..,S1:..,A2:..,F2:..,P2:..,S2:..
*/
static bool send_group(const SignalGroup& g) {
    char buf[160]; // vorher 128, die Zeile ist durch die drei Indizes länger geworden
    snprintf(buf, sizeof(buf),
        "G:%u,T:%u,U:%u,A1:%.2f,F1:%.2f,P1:%.2f,S1:%u,A2:%.2f,F2:%.2f,P2:%.2f,S2:%u\n",
        g.main_group, g.task, g.sub_group,
        g.ch1.amplitude_mV, g.ch1.frequency_Hz, g.ch1.phase_deg, g.ch1.signalType,
        g.ch2.amplitude_mV, g.ch2.frequency_Hz, g.ch2.phase_deg, g.ch2.signalType);

    UART_PORT.print(buf);
    Serial.printf("UART TX: %s", buf);  // Debug-Ausgabe
    return ACK_wait();                  // Warten auf Bestätigung
}

/**
* Übergeordnetes Senden aller Gruppen:
*  1. CFG:M,T,S,N  -> der Slave erfährt die Anzahlen und wie viele Gruppen kommen (N = M*T*S, aus der CSV berechnet)
*  2. N Gruppenzeilen, jede wird einzeln bestätigt
*  3. DONE         -> der Slave prüft, ob er N Gruppen erhalten hat, speichert sie im NVS und bestätigt dann
* Bei jedem fehlenden/negativen ACK wird sofort abgebrochen (false -> rotes UART-Fehler-Popup).
*/
bool uart_send_groups(const SignalGroup* groups, const GroupConfig &cfg, uint32_t total) {
    // Alte, unbeantwortete Zeichen im Empfangspuffer verwerfen, damit kein altes ACK falsch gelesen wird
    while (UART_PORT.available()) UART_PORT.read();

    // 1. Konfiguration senden
    char header[48];
    snprintf(header, sizeof(header), "CFG:%u,%u,%u,%lu\n",
             cfg.mainGroups, cfg.tasks, cfg.subGroups, (unsigned long)total);
    UART_PORT.print(header);
    Serial.printf("UART TX: %s", header);
    if (!ACK_wait(1000)) {
        Serial.println("Fehler bei CFG");
        return false;
    }

    // 2. Alle Gruppen nacheinander senden (Array ist in Indexreihenfolge sortiert)
    for (uint32_t i = 0; i < total; i++) {
        Serial.printf("Sende Gruppe %lu/%lu...\n", (unsigned long)(i + 1), (unsigned long)total);
        if (!send_group(groups[i])) {
            Serial.printf("Fehler bei Gruppe H%u A%u U%u\n",
                          groups[i].main_group, groups[i].task, groups[i].sub_group);
            return false;
        }
    }

    // 3. Ende melden und auf die Bestätigung des Slaves warten (längerer Timeout wegen NVS-Schreiben)
    UART_PORT.print("DONE\n");
    Serial.println("UART TX: DONE");
    return ACK_wait(3000);
}

/**
* Startbefehl: START:<m>,<t>,<s>  (z.B. "START:2,1,2\n")
* Der Slave rechnet daraus die Position aus und startet die passende Gruppe.
*/
bool uart_group_start(uint8_t m, uint8_t t, uint8_t s) {
    char buf[32];
    snprintf(buf, sizeof(buf), "START:%u,%u,%u\n", m, t, s);
    UART_PORT.print(buf);                               // Senden an den ESP32 Dev Modul
    Serial.printf("UART TX: %s", buf);                  // Debug-Ausgabe
    return ACK_wait();
}

bool uart_group_stop() {
    UART_PORT.print("STOP\n");
    Serial.println("UART TX: STOP");
    return ACK_wait();
}