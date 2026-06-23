#ifndef RESISTANCE_H
#define RESISTANCE_H
/**
 * @file resistance.h
 * @brief Header-Datei für die Berechnung des Widerstands basierend auf der vorgegebenen Spannung des Benutzers.
 */

//--------------------------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include <Arduino.h>
//--------------------------------------------------------------------------------------
// Funktion zur Berechnung der Widerstände für die Potentiometer basierend auf der Zielspannung und der Chip-Nummer (0 oder 1).
void calculateResistance(float targetVoltage, int chip);

#endif // RESISTANCE_H