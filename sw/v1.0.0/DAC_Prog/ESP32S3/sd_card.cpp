/**
 * @file sd_card.cpp
 * @brief Funktionen zur Verwaltung der SD-Karte, einschließlich Initialisierung, Überprüfung des Einsteckens und Aktualisierung des Status.
 * Diese Datei enthält die Funktionen sd_init(), sd_card_present() und sd_update(), die für die Verwaltung der SD-Karte verantwortlich sind. 
 * Sie verwendet die SPI- und SD-Bibliotheken, um mit der SD-Karte zu kommunizieren, und aktualisiert die globalen Variablen, um den Status der SD-Karte anzuzeigen.
 */
//----------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>
#include <format>
#include "sd_card.h"
#include "vars.h"
#include <Arduino.h>
#include <string>
#include "buzzer.h"

//----------------------------------------------------------------------

/**
 * Definition der SD-Karten Pins:
 */
#define SD_CS 21
#define SD_SCLK 14
#define SD_MISO 16
#define SD_MOSI 17

//----------------------------------------------------------------------

/**
 * Initialisierung der SD-Karte und Überprüfung, ob sie eingesteckt ist.
 */
SPIClass SD_SPI(HSPI);

//----------------------------------------------------------------------

/**
 * Globale Variablen zum Status der SD-Karte, einschließlich Einsteckstatus und Größe der Karte.
 * Diese Variablen werden über die externen Funktionen get_var_sd_card_inserted(), set_var_sd_card_inserted(), get_var_sd_card_size() und set_var_sd_card_size() verwaltet, um den Status der SD-Karte anzuzeigen.
 */

bool sd_card_inserted;

extern "C" bool get_var_sd_card_inserted() {
  return sd_card_inserted;
}

extern "C" void set_var_sd_card_inserted(bool value) {
  sd_card_inserted = value;
}

std::string sd_card_size;

extern "C" const char *get_var_sd_card_size() {
  return sd_card_size.c_str();
}

extern "C" void set_var_sd_card_size(const char *value) {
  sd_card_size = value;
}

//----------------------------------------------------------------------
/**
 * Initialisiert die SD-Karte, überprüft, ob sie eingesteckt ist, und aktualisiert die globalen Variablen entsprechend.
 * Wenn die SD-Karte erfolgreich initialisiert wird, wird die Größe der Karte in GB berechnet und in der globalen Variable sd_card_size gespeichert und die Variable sd_card_inserted aktualisiert, um anzuzeigen, dass die Karte eingesteckt ist.
 * Wenn die Initialisierung fehlschlägt, werden die entsprechenden Variablen aktualisiert, um anzuzeigen, dass keine SD-Karte eingesteckt ist.
 */
void sd_init() {
  SD_SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS); // Initialisiert die SPI-Verbindung für die SD-Karte

  // Überprüft, ob die SD-Karte erfolgreich initialisiert werden kann. Wenn nicht, werden die entsprechenden Variablen aktualisiert, um anzuzeigen, dass keine SD-Karte eingesteckt ist.
  if (!SD.begin(SD_CS, SD_SPI)) {
    set_var_sd_card_inserted(false);
    set_var_sd_card_size("");
    return;
  }

  set_var_sd_card_inserted(true);
  float size = SD.cardSize()/ 1e9;
  std::string buff = std::format("{:.1f} GB", size);
  set_var_sd_card_size(buff.c_str());
}

/**
 * Überprüft, ob die SD-Karte tatsächlich eingesteckt ist, indem versucht wird, das Root-Verzeichnis zu öffnen.
 * Wenn das Öffnen des Root-Verzeichnisses fehlschlägt, wird angenommen, dass die SD-Karte nicht eingesteckt ist, und die Funktion gibt false zurück. 
 * Wenn das Öffnen erfolgreich ist, wird die Funktion true zurückgeben, um anzuzeigen, dass die SD-Karte eingesteckt ist.
 */
bool sd_card_present() {
  File root = SD.open("/"); // Versucht, das Root-Verzeichnis der SD-Karte zu öffnen, um zu überprüfen, ob die Karte tatsächlich eingesteckt ist
  if (!root) return false;  // Wenn das Öffnen des Root-Verzeichnisses fehlschlägt, wird angenommen, dass die SD-Karte nicht eingesteckt ist
  root.close();             // Schließt das geöffnete Verzeichnis, um Ressourcen freizugeben
  return true;
}

/**
 * Aktualisiert den Status der SD-Karte, indem regelmäßig überprüft wird, ob die Karte eingesteckt oder entfernt wurde.
 * Diese Funktion wird regelmäßig aufgerufen, um den Status der SD-Karte zu aktualisieren.
 */
void sd_update() {
  static uint32_t last_check = 0;           // Statische Variable, um die Zeit des letzten Überprüfungszeitpunkts zu speichern
  if (millis() - last_check < 2000) return; // Überprüft alle 2 Sekunden, ob die SD-Karte eingesteckt oder entfernt wurde, um unnötige Überprüfungen zu vermeiden
  last_check = millis();

  bool currently_inserted = get_var_sd_card_inserted(); // Ruft den aktuellen Einsteckstatus der SD-Karte ab, um zu überprüfen, ob sich der Status geändert hat
  bool card_present;                                    

  // Überprüft, ob die SD-Karte derzeit als eingesteckt markiert ist. Wenn ja, wird die Funktion sd_card_present() aufgerufen, um zu überprüfen, ob die Karte tatsächlich eingesteckt ist.
  // Wenn die Karte derzeit nicht als eingesteckt markiert ist, wird versucht, die SD-Karte zu initialisieren, um zu überprüfen, ob sie jetzt eingesteckt ist.
  if (currently_inserted) {
    card_present = sd_card_present();
  } else {
    card_present = SD.begin(SD_CS, SD_SPI);
  }

  if (card_present == currently_inserted) return; // Wenn der aktuelle Einsteckstatus mit dem vorherigen übereinstimmt, wird keine Aktualisierung durchgeführt, um unnötige Aktualisierungen zu vermeiden

  // Wenn der Einsteckstatus geändert hat, werden die entsprechenden Variablen aktualisiert und die entsprechenden Aktionen ausgeführt, um den neuen Status der SD-Karte anzuzeigen.
  if (card_present) {
    set_var_sd_card_inserted(true);
    float size = SD.cardSize()/ 1e9;
    std::string buff = std::format("{:.1f} GB", size);
    set_var_sd_card_size(buff.c_str());
    buzzer_card_inserted();
  } 
  else {
    SD.end();
    set_var_sd_card_inserted(false);
    set_var_sd_card_size("");
    buzzer_card_removed();
  }
}