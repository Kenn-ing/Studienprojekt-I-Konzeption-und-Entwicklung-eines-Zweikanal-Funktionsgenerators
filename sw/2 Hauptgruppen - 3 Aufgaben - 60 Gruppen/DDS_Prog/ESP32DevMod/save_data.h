/**
 * @file save_data.h
 * @brief Headerdatei für das Speichern im NVS (Non-Volatile Storage)
 * Funktionsdeklarationen für das Speichern und Laden der Gruppen im NVS.
 */
//----------------------------------------------------------------------
#pragma once
#include "signal_config.h"

bool nvs_init();                                                      // NVS Speicher initialisieren
void nvs_save_all(const SignalGroup* groups, uint8_t amount);         // Alle Gruppen speichern
uint8_t nvs_load_all(SignalGroup* groups_out);                        // Alle Gruppen laden, gibt Anzahl zurück
bool nvs_load_group(uint8_t group_id, SignalGroup& out);              // Eine einzelne Gruppe anhand der ID laden