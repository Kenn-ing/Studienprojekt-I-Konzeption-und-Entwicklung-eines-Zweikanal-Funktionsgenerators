/**
 * @file buzzer.h
 * @brief Headerdatei der Funktionen für den Buzzer.
 * Der Buzzer gibt Töne von sich, wenn bestimmte Ereignisse eintreten, wie z.B. das Drücken einer Taste oder das Einlegen/Entfernen einer SD-Karte.
 */
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------

#include <Arduino.h>

//----------------------------------------------------------------------

/**
 * Definition des Buzzer-Pins
 */
#define BUZZER_PIN 15

//----------------------------------------------------------------------

/**
 * Funktionsdeklarationen
 */
void buzzer_init();
void buzzer_sound();
void buzzer_card_inserted();
void buzzer_card_removed();
