#ifndef AD9833_H
#define AD9833_H
/**
 * @file AD9833.h
 * @brief Header-Datei für die Kommunikation mit dem AD9833
 */

//-----------------------------------------------------------------------------------------------------

/**
 * Inkludierte Header-Dateien
 */

#include <Arduino.h>

//-----------------------------------------------------------------------------------------------------

// Pins
#define PIN_FS1  18  // FSYNC (Chip select, active LOW)
#define PIN_FS2   5  // FSYNC (Chip select, active LOW)
#define PIN_SYNC 22  // Pin für synchronisation der beiden Chips
#define PIN_SCLK 21  // SCLK
#define PIN_SD   19  // SDATA

extern const uint32_t MCLK_HZ; // Masterclock des AD9833 (25 MHz)
extern const uint32_t MAX_FREQ; // Maximal einstellbare Frequenz (MCLK/2)

extern uint16_t activeControl[2]; // Aktiver Control, damit z.B das Setzen von Frequenzen oder des Modus den Sleep nicht unterbrechen
extern float phase[2], f[2]; // Aktuell eingestellte Phase und Frequenz
extern int mode[2]; // 0=Sinus, 1=Rechteck


// Commands
#define B28     0x2000  // B28=1: Frequenzregister fungiert als ein 28-bit frequency Register (14 LSB dann 14 MSB)
#define RESET   0x0100  // RESET=1: Alle Register auf Default zurücksetzen
#define SLEEP   0x00C0  // SLEEP: SLEEP=1: DAC und Masterclock abschalten
#define SIN     0x0000  // Sinusausgabe
#define SQU     0x0020  // Rechteckausgabe


//Funktionendeklarationen
void UpdateFreq(float freqHz, int chip);
void UpdatePhase(float phaseDeg, int chip);
void UpdateRegister(uint16_t data, int chip);
float getFreq(int chip);
float getPhase(int chip);
void WriteFreqWord(float freqHz, int chip);
void SyncChips();
void AD9833_Init();

#endif // AD9833_H