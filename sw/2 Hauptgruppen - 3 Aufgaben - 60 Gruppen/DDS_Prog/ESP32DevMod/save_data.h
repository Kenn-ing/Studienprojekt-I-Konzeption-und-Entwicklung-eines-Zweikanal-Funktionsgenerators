/**
 * @file save_data.h
 * @brief Headerdatei für das Speichern im NVS (Non-Volatile Storage)
 * Funktionsdeklarationen für das Speichern und Laden der Gruppen im NVS.
 */
//----------------------------------------------------------------------
#pragma once
#include "signal_config.h"

bool nvs_init();                                                      // NVS Speicher initialisieren
bool nvs_save_all(const SignalGroup* groups, const GroupConfig& cfg, uint32_t amount);   // Alle Gruppen + Anzahlen speichern, false bei Fehler
bool nvs_load_config(GroupConfig& cfg);                                                  // Anzahlen (Hauptgruppen, Aufgaben, Untergruppen) laden
bool nvs_load_group(const GroupConfig& cfg, uint8_t m, uint8_t t, uint8_t s, SignalGroup& out);  // Eine einzelne Gruppe anhand von Hauptgruppe/Aufgabe/Untergruppe laden