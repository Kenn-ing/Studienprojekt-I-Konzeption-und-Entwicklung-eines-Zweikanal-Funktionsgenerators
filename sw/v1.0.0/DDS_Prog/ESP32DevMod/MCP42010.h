#ifndef MCP42010_H
#define MCP42010_H

/**
 * @file MCP42010.h
 * @brief Header-Datei für die Steuerung der MCP42010 digitalen Potentiometer über eine SPI-Schnittstelle.
 */

//-------------------------------------------------------------------------------------

/** 
 * Inkludierte Header-Dateien
 */
#include <Arduino.h>

//-------------------------------------------------------------------------------------

// Pins
#define PIN_CS1   4  // Chip Select Pin 1 für SPI
#define PIN_CS2   0  // Chip Select Pin 2 für SPI
#define PIN_SCK   25  // SCLK
#define PIN_SI    26  // SDATA

// Commands
#define CMD_POT0 0x11 // Kommando fürs schreiben auf Potentiometer 0 (00010001)
#define CMD_POT1 0x12 // Kommando fürs schreiben auf Potentiometer 1 (00010010)
#define CMD_BOTH 0x13 // Kommando fürs schreiben auf beide Potentiometer (00010011)

// DS1803 Widerstände pro Chips
extern const float P0_255[2];
extern const float P0_0[2];
extern const float P1_255[2];
extern const float P1_0[2];


// Zustandsvariablen (Arrays für zwei Chips)
extern uint8_t positionPot0[2];  // Aktuelle Position von Potentiometer 0 (0–255)
extern uint8_t positionPot1[2];  // Aktuelle Position von Potentiometer 1 (0–255)

//Funktionen für die Potentiometersteuerung
void setPot0(int value, int chip);
void setPot1(int value, int chip);
void setBoth(int value, int chip);
void MCP42010_Init();

#endif // MCP42010_H