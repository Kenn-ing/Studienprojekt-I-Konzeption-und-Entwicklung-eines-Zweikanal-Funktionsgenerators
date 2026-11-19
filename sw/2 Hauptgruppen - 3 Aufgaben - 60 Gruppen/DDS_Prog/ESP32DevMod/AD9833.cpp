/**
* @file AD9833.cpp
* @brief Datei für die Kommunikation mit dem AD9833
* Diese Datei implementiert die Funktionen zur Kommunikation mit dem AD9833, einschließlich der Aktualisierung von Frequenz, Phase und Register-Status.
*/

//--------------------------------------------------------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */

#include "AD9833.h"

//--------------------------------------------------------------------------------------------------------------------


const uint32_t MCLK_HZ = 25000000UL;    // Masterclock des AD9833 (25 MHz)
const uint32_t MAX_FREQ = MCLK_HZ / 2;  // Laut Datenblatt die maximale Ausgangsfrequenz des AD9833 (12.5 MHz)

// Timing-Delays (microseconds)
const uint8_t T_DELAY_US = 1;

uint16_t activeControl[2] = { B28, B28 };                // Aktiver Control, damit z.B das Setzen von Frequenzen oder des Modus den Sleep nicht unterbrechen
float phase[2] = { 0.0f, 0.0f }, f[2] = { 0.0f, 0.0f };  // Aktuell eingestellte Phase und Frequenz
int mode[2] = { 0, 0 };                                  // 0=Sinus, 1=Rechteck

/**
 * Funktion zum Abfragen des FSYNC-Pins basierend auf der Chip-Nummer (0 oder 1).
 */
int getFSPin(int chip) {
  if (chip == 0) return PIN_FS1;
  else return PIN_FS2;
}

/**
 * Funktion zum Senden eines 16-Bit Datenworts an den AD9833.
 * Die Daten werden MSB-first übertragen, beginnend mit einem aktiven FSYNC (LOW) und endend mit einem inaktiven FSYNC (HIGH).
 */
void UpdateRegister(uint16_t data, int chip) {
  // Abfrage, welcher FSYNC-Pin für den jeweiligen Chip verwendet werden soll
  uint8_t fsPin = getFSPin(chip);

  // Startbedingung SCLK=High, FSYNC=LOW
  digitalWrite(PIN_SCLK, HIGH);
  delayMicroseconds(T_DELAY_US);

  digitalWrite(fsPin, LOW);  // FSYNC active LOW. Übertragung starten
  delayMicroseconds(T_DELAY_US);

  // Daten MSB-first übertragen
  for (int bit = 15; bit >= 0; --bit) {
    // set data
    if ((data >> bit) & 0x1) digitalWrite(PIN_SD, HIGH);
    else digitalWrite(PIN_SD, LOW);


    delayMicroseconds(T_DELAY_US);
    digitalWrite(PIN_SCLK, LOW);  // fallende Flanke
    delayMicroseconds(T_DELAY_US);
    digitalWrite(PIN_SCLK, HIGH);  // steigende Flanke
    delayMicroseconds(T_DELAY_US);
  }

  // Übertragung beenden (FSYNC=HIGH)
  digitalWrite(fsPin, HIGH);
  delayMicroseconds(T_DELAY_US);
}

/**
 * Funktion zum Aktualisieren der Frequenz des AD9833.
 * Die Funktion berechnet das 28-bit FREQWORD basierend auf der gewünschten Frequenz und der Masterclock, teilt es in LSB und MSB auf und sendet die entsprechenden Register-Updates.
 */
void UpdateFreq(float freqHz, int chip) {
  // Schutzbereich
  if (freqHz < 0.0f) freqHz = 0.0f;
  if (freqHz > float(MAX_FREQ)) freqHz = float(MAX_FREQ);  // max. 12.5 MHz

  // Wenn Rechteckspannung ausgegeben werden soll, muss die Frequenz verdoppelt werden
  if (mode[chip] == 1) freqHz = freqHz * 2;

  // Berechnung 28-bit FREQWORD: round((freq * 2^28) / MCLK)
  double freqWordDbl = ((double)freqHz * (double)(1ULL << 28)) / (double)MCLK_HZ;

  uint32_t freqWord = (uint32_t)llround(freqWordDbl);  // Runden

  // teile in 14 LSB / 14 MSB
  uint16_t LSB14 = freqWord & 0x3FFF;          // Bits 13..0
  uint16_t MSB14 = (freqWord >> 14) & 0x3FFF;  // Bits 27..14

  // Adresse für FREQ0 (D15D14 = 01) -> 0x4000 | 14bitdata
  uint16_t wordLSB = 0x4000 | LSB14;
  uint16_t wordMSB = 0x4000 | MSB14;

  // Reset=1
  UpdateRegister(activeControl[chip] |= RESET, chip);

  // LSB dann MSB übertragen
  UpdateRegister(wordLSB, chip);
  UpdateRegister(wordMSB, chip);

  // Sinus-Wellenform (B28=0, RESET=0, Mode=0)
  if (mode[chip] == 0) {
    UpdateRegister((activeControl[chip] &= ~RESET) & ~SQU, chip);  // Sinus-Wellenform
  }
  // Rechteck-Wellenform (B28=0, RESET=0, MODE=1)
  else if (mode[chip] == 1) {
    UpdateRegister((activeControl[chip] &= ~RESET) |= SQU, chip);  // Rechteck-Wellenform
  }
}

/**
 * Funktion zum Aktualisieren der Phase des AD9833.
 * Die Funktion berechnet das 12-bit PHASEWORD basierend auf der gewünschten Phase in Grad, begrenzt den Wertebereich auf 0-360 Grad und sendet das entsprechende Register-Update.
 */
void UpdatePhase(float phaseDeg, int chip) {
  // Wertebereich begrenzen
  if (phaseDeg < 0.0f) phaseDeg = 0.0f;
  if (phaseDeg > 360.0f) phaseDeg = 360.0f;

  if (mode[chip] == 1) phaseDeg = phaseDeg * 2;

  // 12-bit Phase Word berechnen
  uint16_t phaseWord = (uint16_t)((phaseDeg * 4096.0f / 360.0f) + 0.5f);
  phaseWord &= 0x0FFF;

  // PHASE0 schreiben
  uint16_t reg = 0xC000 | phaseWord;
  UpdateRegister(reg, chip);
}

/**
 * Hilfsfunktion zum Abfragen der aktuell eingestellten Frequenz
 */
float getFreq(int chip) {
  return f[chip];
}

/**
 * Hilfsfunktion zum Abfragen der aktuell eingestellten Phase
 */
float getPhase(int chip) {
  return phase[chip];
}

/**
 * Funktion zum Schreiben eines Frequenzworts (28-bit) für die Synchronisation beider Chips.
 */
void WriteFreqWord(float freqHz, int chip) {
  // Schutzbereich identisch zur oberen Funktion UpdateFreq
  if (freqHz < 0.0f) freqHz = 0.0f;
  if (freqHz > float(MAX_FREQ)) freqHz = float(MAX_FREQ);
  if (mode[chip] == 1) freqHz = freqHz * 2;

  // Berechnung 28-bit FREQWORD: round((freq * 2^28) / MCLK)
  double freqWordDbl = ((double)freqHz * (double)(1ULL << 28)) / (double)MCLK_HZ;
  uint32_t freqWord = (uint32_t)llround(freqWordDbl);
  uint16_t wordLSB = 0x4000 | (freqWord & 0x3FFF);
  uint16_t wordMSB = 0x4000 | ((freqWord >> 14) & 0x3FFF);

  // Updaten der beiden Register
  UpdateRegister(wordLSB, chip);
  UpdateRegister(wordMSB, chip);
}

/**
 * Funktion zum Synchronisieren beider AD9833-Chips.
 * Diese Funktion setzt beide Chips gleichzeitig zurück und überträgt die gleichen Frequenzdaten, um sicherzustellen, dass beide Chips synchron laufen.
 */
void SyncChips() {
  // Beide einzeln mit RESET=1 und Frequenz übertragen
  UpdateRegister(activeControl[0] |= RESET, 0);
  UpdateRegister(activeControl[1] |= RESET, 1);
  // Frequenzwörter übertragen
  WriteFreqWord(f[0], 0);
  WriteFreqWord(f[1], 1);

  // activeControl für Modus anpassen
  for (int chip = 0; chip < 2; chip++) {
    activeControl[chip] &= ~RESET;
    if (mode[chip] == 1) activeControl[chip] |= SQU;
    else activeControl[chip] &= ~SQU;
  }


  // Über den gemeinsamen Sync-Pin beide Chips gleichzeitig mit den gleichen Daten beschreiben, damit sie beide synchron laufen
  if (activeControl[0] == activeControl[1]) {
    digitalWrite(PIN_SCLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(PIN_SYNC, LOW);  // Übertragung starten
    delayMicroseconds(1);

    // 16-Bit Control-Word seriell übertragen (MSB zuerst)
    for (int bit = 15; bit >= 0; --bit) {
      digitalWrite(PIN_SD, (activeControl[0] >> bit) & 0x1 ? HIGH : LOW);
      delayMicroseconds(1);
      digitalWrite(PIN_SCLK, LOW);
      delayMicroseconds(1);
      digitalWrite(PIN_SCLK, HIGH);
      delayMicroseconds(1);
    }
    // Übertragung beenden
    digitalWrite(PIN_SYNC, HIGH);
  }
  // Unterschiedliche Modi, dann sequenziell so schnell wie möglich freigeben
  else {
    UpdateRegister(activeControl[0], 0);
    UpdateRegister(activeControl[1], 1);
  }
}

/**
 * Funktion zum Initialisieren der AD9833-Chips.
 * Diese Funktion konfiguriert die Pins, setzt den initialen Control-Status und aktiviert den Sleep-Modus, um die Chips in einen definierten Zustand zu versetzen.
 */
void AD9833_Init() {
  pinMode(PIN_FS1, OUTPUT);
  pinMode(PIN_FS2, OUTPUT);
  pinMode(PIN_SYNC, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  pinMode(PIN_SD, OUTPUT);
  digitalWrite(PIN_FS1, HIGH);
  digitalWrite(PIN_FS2, HIGH);
  digitalWrite(PIN_SYNC, HIGH);
  digitalWrite(PIN_SCLK, HIGH);
  digitalWrite(PIN_SD, LOW);

  UpdateRegister(activeControl[0] |= RESET, 0);
  UpdateRegister((activeControl[0] &= ~RESET) |= SLEEP, 0);
  UpdateRegister(activeControl[1] |= RESET, 1);
  UpdateRegister((activeControl[1] &= ~RESET) |= SLEEP, 1);

  SyncChips();
}
