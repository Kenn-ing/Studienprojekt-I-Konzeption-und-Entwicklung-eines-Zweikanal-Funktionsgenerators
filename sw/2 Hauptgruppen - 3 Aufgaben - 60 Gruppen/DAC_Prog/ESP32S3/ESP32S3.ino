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
#include <string>
#include "ui_config.h"

/**
 * Globaler Variablenbereich für die Versionnummer des Projektes. Diese Variable wird über die Funktionen get_var_version_number_text() und set_var_version_number_text() verwaltet, um den Zugriff auf die Versionnummer zu ermöglichen.
 */
std::string version_number_text;

extern "C" const char *get_var_version_number_text() {
    return version_number_text.c_str();
}

extern "C" void set_var_version_number_text(const char *value) {
    version_number_text = value;
}

//----------------------------------------------------------------------

/**
 * Setup-Funktion, die einmalig beim Start des Programms aufgerufen wird. Hier werden die serielle Kommunikation, die Hintergrundbeleuchtung, das LCD-Display, die LVGL-Bibliothek, die SD-Karte, die Benutzeroberfläche und der Buzzer initialisiert. 
 * Nach einer kurzen Anzeige des Startbildschirms wird der Hauptbildschirm geladen.
 * Daurauf folgend kann das HMI genutzt werden und zum Beispiel die Daten von der SD Karte kann gelesen werden
 */
void setup() {
  set_var_version_number_text("v2.0.0");
  Serial.begin(115200);
  Backlight_Init();
  LCD_Init();
  Lvgl_Init();
  sd_init();
  ui_init();
  ui_config_init_from_nvs();
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
