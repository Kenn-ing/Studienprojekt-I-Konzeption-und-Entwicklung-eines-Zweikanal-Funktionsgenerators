/**
 * @file nvs_store.h
 * @brief KOMPLETT NEU (Master): Speichern/Laden der Anzahlen (Hauptgruppen, Aufgaben, Untergruppen)
 * im nichtflüchtigen Speicher (NVS) des ESP32-S3.
 */
#pragma once
#include <Arduino.h>
#include "signal_config.h"

// Speichert die drei Anzahlen im NVS (nur wenn sie sich geändert haben)
void nvs_save_config(const GroupConfig &c);

// Lädt die Anzahlen aus dem NVS. false, wenn noch nichts gespeichert oder ungültig.
bool nvs_load_config(GroupConfig &c);
