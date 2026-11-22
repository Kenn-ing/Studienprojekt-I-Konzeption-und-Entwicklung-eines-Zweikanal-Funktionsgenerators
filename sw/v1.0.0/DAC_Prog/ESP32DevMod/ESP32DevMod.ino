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
#include "dac_output.h"
#include "ws2812b.h"

static SignalGroup groups[MAX_GROUPS];
static uint8_t anzahl_gruppen = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  ws2812b_init();
  Serial.println("ESP32 Dev Modul gestartet");

  // Initialisieren vom NVS, laden der Gruppen
  nvs_init();
  dac_output_init();
  uart_init();

  // Ausgabe der vorhandenen Gruppen im Serial Monitor
  anzahl_gruppen = nvs_load_all(groups);
  if (anzahl_gruppen > 0) {
    Serial.printf("%u Gruppen aus NVS geladen, warte auf Startsignal.\n", anzahl_gruppen);
  }
}

void loop() {

  // Ausgeben der Funktion und warten auf Übertragung neuer Gruppen
  uart_loop();
  dac_output_loop();
}