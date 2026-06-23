#ifndef WS2812B_H
#define WS2812B_H
/**
 * @file ws2812b.h
 * @brief Diese Datei enthält die Deklaration der Funktionen zur Steuerung der WS2812B-LEDs.
 */

//-------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include <Arduino.h>

//-------------------------------------------------------------------

// Definition der Pin-Nummer und der Anzahl der LEDs in der Kette
#define WS2812B_PIN 15
#define LED_COUNT 12

// Deklaration der Funktionen
void ws2812b_init();
void dac_activated();
void dac_deactivated();


#endif  // WS2812B_H