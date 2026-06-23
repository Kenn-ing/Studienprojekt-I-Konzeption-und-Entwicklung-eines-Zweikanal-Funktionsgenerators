/**
 * @file csv.h
 * @brief Header-Datei zum Einlesen der CSV-Datei mit den DAC-Werten für die HMI-Anwendung auf dem ESP32S3. 
 * Die einzelnen Funktionen zum Einlesen und Filtern werden hier deklariert.
 * Die CSV-Datei enthält die Werte für die DAC-Ausgabe, die von der HMI-Anwendung verwendet werden, 
 * um die entsprechenden Signale zu generieren.
 */
//-------------------------------------------------------------
#pragma once
#include <Arduino.h>
#include <SD.h>
#include "signal_config.h"

// Dateiname der CSV auf der SD-Karte
static constexpr char CSV_FILENAME[] = "/GruppenDAC.csv";

// Initialisiert die SD-Karte und den SPI-Bus
bool csv_init(uint8_t cs_pin);

/**
 * Liest alle Gruppen aus der CSV-Datei und speichert sie im Array.
 * Erste Zeile (Header) wird übersprungen.
 * Leere Zeilen werden ignoriert.
 */
bool csv_read(SignalGroup groups_array[], uint8_t &group_count);

// Gibt alle Parameter einer Gruppe auf dem Serial Monitor aus (nur Debugging)
void csv_debug(const SignalGroup &group);