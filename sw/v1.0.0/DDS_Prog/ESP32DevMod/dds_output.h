#ifndef DDS_OUTPUT_H
#define DDS_OUTPUT_H
/**
 * @file dds_output.h
 * @brief Datei für die DDS-Ausgabe
 */

//-------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include <Arduino.h>
#include "signal_config.h"

//-------------------------------------------------------------------

//Deklaration der Funktionen
void dds_start(const SignalGroup* gruppen, uint8_t anzahl);
void dds_stop();

#endif // DDS_OUTPUT_H