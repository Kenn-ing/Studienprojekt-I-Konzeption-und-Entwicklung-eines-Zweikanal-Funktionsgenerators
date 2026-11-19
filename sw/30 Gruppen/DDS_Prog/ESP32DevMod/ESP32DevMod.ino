/**
 * @file ESP32DEVMOD.ino
 * @brief Hauptdatei des Projektes. 
 * Läd die gespeicherten Gruppen aus dem Speicher, wartet auf Signale vom ESP32S3 und erzeugt über die DAC-Pins die Funktionen
 * Ruft die essentiellen Funktionen auf und inkludiert alle benötigten Header-Dateien
 */
//----------------------------------------------------------------------
#include "signal_config.h"
#include "uart.h"
#include "save_data.h"
#include "MCP42010.h"
#include "AD9833.h"
#include "ws2812b.h"

static SignalGroup gruppen[MAX_GROUPS];
static uint8_t    anzahl_gruppen = 0;

void setup() {
    Serial.begin(115200);
    delay(500);
    ws2812b_init();
    Serial.println("ESP32 develop gestartet");

    nvs_init();
    uart_init();
    AD9833_Init();
    MCP42010_Init();

    // Ausgabe der vorhandenen Gruppen im Serial Monitor
    anzahl_gruppen = nvs_load_all(gruppen);
    if (anzahl_gruppen > 0) {
        Serial.printf("%u Gruppen aus NVS geladen, warte auf Startsignal.\n", anzahl_gruppen);
    }
}

void loop() {
// Warten auf Übertragung neuer Gruppen
    uart_loop();  
}