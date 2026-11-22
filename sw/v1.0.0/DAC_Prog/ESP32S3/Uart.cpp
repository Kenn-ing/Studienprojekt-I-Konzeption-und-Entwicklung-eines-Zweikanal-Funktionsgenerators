/**
 * @file Uart.cpp
 * @brief Umsetzung der UART Kommunikation
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
* Wartet auf "ACK" oder "NACK" vom Empfänger
* Wenn die Funktion "true" zurück gibt, dann war die Übertragung von Daten oder einem Start-/Stoppsignal erfolgreich
* Sollte es zu einem Fehler kommen dann wird eine Fehlermeldung auf dem Serial Monitor ausgegeben
*/ 
static bool ACK_wait() {
    unsigned long t = millis();
    // Prüfen ob eine Antwort kommt oder die 500 ms abgelaufen sind
    while (millis() - t < UART_ACK_TIMEOUT_MS) {
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
* Verpacken der Daten und das übertragen den ESP32 Dev Modul
*/
static bool send_groups(const SignalGroup& g) {
    char buf[128]; //Puffer für das Senden
    snprintf(buf, sizeof(buf), //String formatieren,alle Informationen der Gruppe werden in das folgende Format verpackt und anschließend gesendet
        "G:%u,A1:%.2f,F1:%.2f,P1:%.2f,S1:%u,A2:%.2f,F2:%.2f,P2:%.2f,S2:%u\n",
        g.group_id,                                          
        g.ch1.amplitude_mV, g.ch1.frequency_Hz, g.ch1.phase_deg, g.ch1.signalType, 
        g.ch2.amplitude_mV, g.ch2.frequency_Hz, g.ch2.phase_deg, g.ch2.signalType); 

    UART_PORT.print(buf);
    Serial.printf("UART TX: %s", buf);  // Debug-Ausgabe 
    return ACK_wait();                  // Warten auf Bestätigung
}

/** 
* Funktion für ein übergeordnetes Senden
* Übertragung von mehreren Gruppen hintereinander 
*/
bool uart_send_groups(const SignalGroup* groups, uint8_t amount) {
    // Ankündigung: Anzahl der folgenden Gruppen und senden des Headers
    char header[32];
    snprintf(header, sizeof(header), "COUNT:%u\n", amount);
    UART_PORT.print(header);
    delay(50);  // kurze Pause damit der Empfänger bereit ist

    for (uint8_t i = 0; i < amount; i++) {
        Serial.printf("Sende Gruppe %u/%u...\n", i + 1, amount);
        
        bool success = send_groups(groups[i]);  //  Funktion aufrufen, Ergebnis speichern
        
        if (success == false) {                 //  Ergebnis prüfen
            Serial.printf("Fehler bei Gruppe %u\n", groups[i].group_id);
            return false;
        }
    }

    UART_PORT.print("DONE\n");
    return true;
}

/**
* Start und Stoppbefehl für das Senden der Daten
*/
bool uart_group_start(uint8_t group_id) {
    char buf[32];
    snprintf(buf, sizeof(buf), "START:%u\n", group_id); // Formatiert die Nachricht "START:1\n" (mit group_id)
    UART_PORT.print(buf);                               // Senden an den ESP32 Dev Modul
    Serial.printf("UART TX: %s", buf);                  // Debug-Ausgabe
    return ACK_wait();
}

bool uart_group_stop() {
    UART_PORT.print("STOP\n");
    Serial.println("UART TX: STOP");
    return ACK_wait();
}