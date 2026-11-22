/**
 * @file ui_config.h
 * @brief KOMPLETT NEU (Master): Befüllt die Dropdowns der UI anhand der Anzahlen aus der CSV.
 */
#pragma once
#include "signal_config.h"

// Baut die Optionslisten ("1\n2\n3...") und übergibt sie an die EEZ-Variablen. Setzt die Auswahl auf den ersten Eintrag.
void ui_apply_config(const GroupConfig &c);

// Im setup() nach der UI-Initialisierung aufrufen: lädt die Anzahlen aus dem NVS und befüllt die Dropdowns.
void ui_config_init_from_nvs();
