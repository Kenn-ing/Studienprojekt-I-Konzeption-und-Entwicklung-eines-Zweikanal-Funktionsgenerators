/**
 * @file buzzer.cpp
 * @brief Implementierung der Funktionen für den Buzzer.
 * Der Buzzer gibt Töne von sich, wenn bestimmte Ereignisse eintreten, wie z.B. das Drücken einer Taste oder das Einlegen/Entfernen einer SD-Karte.
 */
//----------------------------------------------------------------------

#include "buzzer.h"

//----------------------------------------------------------------------

/**
 * Initialisiert den Buzzer-Pin als Ausgang.
 */
void buzzer_init(){
    pinMode(BUZZER_PIN, OUTPUT);
}

/**
 * Gibt einen Ton vom Buzzer aus, wenn eine Taste gedrückt wird.
 */
void buzzer_sound(){
  tone(BUZZER_PIN, 3610, 50);
}

/**
 * Gibt einen Ton aus, wenn eine SD-Karte eingelegt wird.
 */
void buzzer_card_inserted(){
  tone(BUZZER_PIN, 2460, 100);
  tone(BUZZER_PIN, 2657, 100);

}

/**
 * Gibt einen Ton aus, wenn eine SD-Karte entfernt wird.
 */
void buzzer_card_removed(){
  tone(BUZZER_PIN, 2657, 100);
  tone(BUZZER_PIN, 2460, 100);
}
