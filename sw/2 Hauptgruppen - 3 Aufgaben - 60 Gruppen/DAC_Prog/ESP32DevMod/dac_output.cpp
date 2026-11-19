/**
 * @file dac_outout.cpp
 * @brief Ausgabe der beiden Funktionen über die DAC Pins
 * Hier werden nach dem Startsignal die Ausgabe der Funktionen verarbeitet 
 * Die Ausagbe läuft solange bis ein Stoppsignal vom HMI übermittelt wird
 */
//----------------------------------------------------------------------
#include "dac_output.h"
#include "ws2812b.h"
#include <Arduino.h>
#include <math.h>

static const float MAX_VREF = 1.5f;
static const float MAX_HALF_COUNTS = 127.5f;

static float f_CH1 = 0.0f, f_CH2 = 0.0f;
static float amp1 = 0.0f, amp2 = 0.0f;  // in counts
static float phi1_rad = 0.0f, phi2_rad = 0.0f;
static int waveMode1 = 0;  // 0 = Sinus, 1 = Rechteck
static int waveMode2 = 0;
static bool aktiv = false;

// Phasenakkumulatoren — speichern die aktuelle Phase beider Kanäle
static float phi_acc1 = 0.0f;
static float phi_acc2 = 0.0f;
static uint32_t lastMicros = 0;  // Zeitbasis für dt-Berechnung

//Hilfsfunktion: mV -> DAC-counts
static float mv_to_counts(float amplitude_mV) {
  float volt = amplitude_mV / 1000.0f;
  if (volt > MAX_VREF) volt = MAX_VREF;
  return volt / MAX_VREF * MAX_HALF_COUNTS;
}

/**
 * Initialisiert die DAC-Pins.
 * Setzt beide Kanäle auf den Mittelwert (128) → nach Kondensator = 0V
 */
void dac_output_init() {
  dacWrite(DAC_PIN_CH1, 128);
  dacWrite(DAC_PIN_CH2, 128);
  Serial.println("DAC Output bereit");
}

// ─────────────────────────────────────────────────────────
void dac_start_output(const SignalGroup* gruppen, uint8_t anzahl) {
  // Ausgabe einer Gruppe
  const SignalGroup& g = gruppen[0];

  // Signalparameter übernehmen
  f_CH1 = g.ch1.frequency_Hz;
  f_CH2 = g.ch2.frequency_Hz;
  amp1 = mv_to_counts(g.ch1.amplitude_mV);
  amp2 = mv_to_counts(g.ch2.amplitude_mV);
  waveMode1 = g.ch1.signalType;
  waveMode2 = g.ch2.signalType;

  // Phasenakkumulatoren mit Startphase initialisieren
  phi_acc1 = -g.ch1.phase_deg * (PI / 180.0f);  // Grad in Radiant umrechnen
  phi_acc2 = -g.ch2.phase_deg * (PI / 180.0f);
  lastMicros = micros();  // Zeitbasis neu starten → beide Kanäle synchron
  aktiv = true;

  dac_activated();

  // Augabe der Information im Serial Monitor
  Serial.printf("DAC start | G%u | CH1: %.1fmV %.1fHz %.1f° Signalform: %hhu\n | CH2: %.1fmV %.1fHz %.1f° Signalform: %hhu\n ",
                g.group_id,
                g.ch1.amplitude_mV, g.ch1.frequency_Hz, g.ch1.phase_deg, g.ch1.signalType,
                g.ch2.amplitude_mV, g.ch2.frequency_Hz, g.ch2.phase_deg, g.ch2.signalType);
}
/**
 * Stoppt die DAC-Ausgabe.
 * Setzt beide Kanäle auf den Mittelwert (128) → nach Kondensator = 0V
 */
void dac_stop_output() {
  aktiv = false;
  dac_deactivated();
  dacWrite(DAC_PIN_CH1, 128);
  dacWrite(DAC_PIN_CH2, 128);
  Serial.println("DAC gestoppt");
}

/**
 * Hauptschleife der DAC-Ausgabe — wird kontinuierlich in loop() aufgerufen.
 * Berechnet die aktuellen DAC-Werte anhand der Phasenakkumulatoren.
 * Phasenakkumulator-Methode: kein Overflow-Problem, beide Kanäle bleiben synchron.
 */
void dac_output_loop() {
  if (!aktiv) return;

  // Zeitdifferenz seit letztem Aufruf berechnen
  uint32_t now = micros();
  uint32_t dtUs = now - lastMicros;  
  lastMicros = now;
  float dt = dtUs * 1e-6f;  // Umrechnung in Sekunden

  // Phasenakkumulatoren aktualisieren
  phi_acc1 += 2.0f * PI * f_CH1 * dt;
  phi_acc2 += 2.0f * PI * f_CH2 * dt;

  // Im Bereich 0..2pi halten damit die Werte nicht unendlich wachsen
  if (phi_acc1 >= 2.0f * PI) phi_acc1 -= 2.0f * PI;
  if (phi_acc2 >= 2.0f * PI) phi_acc2 -= 2.0f * PI;

  float center = MAX_HALF_COUNTS;
  float wave1, wave2;

  // CH1 — Sinus oder Rechteck
  if (waveMode1 == 0) {
    wave1 = sinf(phi_acc1);  // Sinussignal
  } else {
    wave1 = (sinf(phi_acc1) >= 0.0f) ? 1.0f : -1.0f;  // Rechtecksignal
  }

  // CH2 — Sinus oder Rechteck
  if (waveMode2 == 0) {
    wave2 = sinf(phi_acc2);  // Sinussignal
  } else {
    wave2 = (sinf(phi_acc2) >= 0.0f) ? 1.0f : -1.0f;  // Rechtecksignal
  }

  // DAC-Werte ausgeben — begrenzt auf 0..255
  dacWrite(DAC_PIN_CH1, constrain((int)roundf(center + amp1 * wave1), 0, 255));
  dacWrite(DAC_PIN_CH2, constrain((int)roundf(center + amp2 * wave2), 0, 255));
}