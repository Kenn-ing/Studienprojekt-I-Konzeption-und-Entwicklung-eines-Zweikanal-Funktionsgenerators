/**
 * @file dac.h
 * @brief Headerdatei für die Ausgabe der Funktiom
 * Aufrufen der Funktionen für die notwendige Ausgabe
 */
//----------------------------------------------------------------------
#pragma once
#include "signal_config.h"

void dac_output_init();
void dac_start_output(const SignalGroup* gruppen, uint8_t anzahl);
void dac_stop_output();
void dac_output_loop();