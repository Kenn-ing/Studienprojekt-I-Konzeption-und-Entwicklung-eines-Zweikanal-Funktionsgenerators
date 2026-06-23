/**
 * @file ESP32S3.ino
 * @brief Hauptdatei des Projektes. 
 * Initialisiert die Hardware, lädt den Hauptbildschirm und verwaltet die Hauptschleife.
 * Ruft die essentiellen Funktionen auf und inkludiert alle benötigten Header-Dateien
 */
//----------------------------------------------------------------------
#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
#include "vars.h"
#include "buzzer.h"
#include "sd_card.h"
#include "csv.h"
#include "Uart.h"

/**
 * Globaler Variablenbereich für die aktive Gruppe. Diese Variable wird verwendet, um die aktuell ausgewählte Gruppe in der Benutzeroberfläche zu verfolgen. 
 * Sie kann von anderen Teilen des Programms über die bereitgestellten Getter- und Setter-Funktionen abgerufen und geändert werden.
 */
int32_t active_group = 0;

extern "C" int32_t get_var_active_group() {
  return active_group;
}

extern "C" void set_var_active_group(int32_t value) {
  active_group = value;
}

//----------------------------------------------------------------------

/**
 * Setup-Funktion, die einmalig beim Start des Programms aufgerufen wird. Hier werden die serielle Kommunikation, die Hintergrundbeleuchtung, das LCD-Display, die LVGL-Bibliothek, die SD-Karte, die Benutzeroberfläche und der Buzzer initialisiert. 
 * Nach einer kurzen Anzeige des Startbildschirms wird der Hauptbildschirm geladen.
 * Daurauf folgend kann das HMI genutzt werden und zum Beispiel die Daten von der SD Karte kann gelesen werden
 */
void setup() {
  Serial.begin(115200);
  Backlight_Init();
  LCD_Init();
  Lvgl_Init();
  sd_init();
  ui_init();
  buzzer_init();
  uart_init();

  // ca. 500ms Starting-Screen anzeigen
  for (int i = 0; i < 50; i++) {
    Lvgl_Loop();
    delay(10);
  }
  // Hauptbildschirm laden
  loadScreen(SCREEN_ID_MAIN);
}

/**
 * Loop-Funktion, die kontinuierlich ausgeführt wird. Hier werden die LVGL-Loop-Funktion, die ui_tick-Funktion und die sd_update-Funktion aufgerufen, um die Benutzeroberfläche zu aktualisieren und den SD-Karten-Status zu überwachen. 
 */
void loop() {
  Lvgl_Loop();
  ui_tick();
  sd_update();
  vTaskDelay(pdMS_TO_TICKS(1));
}
