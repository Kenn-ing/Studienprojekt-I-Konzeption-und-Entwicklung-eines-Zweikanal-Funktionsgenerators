/**
 * @file signal_config.h
 * @brief Header-Datei zur Definition der Signalkonfigurationen für die HMI-Anwendung auf dem ESP32S3.
 * Gemeinsame Typdefinitionen für ESP32S3 und ESP32 Dev Modul
 */
//----------------------------------------------------------------------
#pragma once
#include <Arduino.h>

// Parameter eines einzelnen DAC-Signals (ein Kanal)
struct SignalParams {
    float      amplitude_mV;  // Amplitude in mV
    float      frequency_Hz;  // Frequenz in Hz
    float      phase_deg;     // Phasenversatz in DEG
    uint8_t    signalType;   // Signalform (Sinus / Viereck)
};

// Eine vollständige Gruppe mit 2 Kanälen
struct SignalGroup {
    uint8_t     group_id;  // Gruppennummer aus CSV  (1 - 30)
    SignalParams ch1;      // Parameter für Kanal 1
    SignalParams ch2;      // Parameter für Kanal 2
};

// Maximale Anzahl Gruppen (CSV hat 30 Einträge)
static constexpr uint8_t MAX_GROUPS = 30;

// Interner ESP32 DAC - 8-bit Auflösung (PWM mit 0 bis 255 Stufen)
static constexpr uint8_t  DAC_RESOLUTION      = 255;

// Interner ESP32 DAC - Spannungsbereich in mV
static constexpr float    DAC_VOLTAGE_MIN_mV  = 0.0f; 
static constexpr float    DAC_VOLTAGE_MAX_mV  = 3000.0f;

// Interne DAC Pins des ESP32 (GPIO25 und GPIO26  DAC Pins)
static constexpr uint8_t  DAC_PIN_CH1         = 25;  // DAC1 -> Kanal 1
static constexpr uint8_t  DAC_PIN_CH2         = 26;  // DAC2 -> Kanal 2

