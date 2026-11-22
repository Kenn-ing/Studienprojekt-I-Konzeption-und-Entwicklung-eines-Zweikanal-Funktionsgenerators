/**
 * @file signal_config.h
 * @brief Header-Datei zur Definition der Signalkonfigurationen für die HMI-Anwendung auf dem ESP32S3.
 * Gemeinsame Typdefinitionen für ESP32S3 und ESP32 Dev Modul
 */
//----------------------------------------------------------------------
#pragma once
#include <Arduino.h>

// Struktur zum Speichern der Gesamtzahl der Hauptgruppen, Aufgaben und Untergruppen
struct GroupConfig{
    uint8_t mainGroups;
    uint8_t tasks;
    uint8_t subGroups;
};

// Parameter eines einzelnen DAC-Signals (ein Kanal)
struct SignalParams {
    float      amplitude_mV;  // Amplitude in mV
    float      frequency_Hz;  // Frequenz in Hz
    float      phase_deg;     // Phasenversatz in DEG
    uint8_t    signalType;   // Signalform (Sinus / Viereck)
};

// Eine vollständige Gruppe mit 2 Kanälen
struct SignalGroup {
    uint8_t main_group;  // Hauptgruppennummer aus CSV  
    uint8_t task;        // Aufgabennummer aus CSV  
    uint8_t sub_group;   // Untergruppennummer aus CSV 
    SignalParams ch1;      // Parameter für Kanal 1
    SignalParams ch2;      // Parameter für Kanal 2
};

// Hilfsfunktionen zur Berechnung der Gesamtzahl der Gruppen auf Basis der Konfiguration
inline uint32_t group_total(const GroupConfig &c) {
    return (uint32_t)c.mainGroups * c.tasks * c.subGroups;
}

// Hilfsfunktion zur Berechnung des Indexes einer Gruppe im Array auf Basis der Konfiguration und der Gruppenangaben (m,t,s)
inline uint32_t group_index(const GroupConfig &c, uint8_t m, uint8_t t, uint8_t s) {
    return ((uint32_t)(m - 1) * c.tasks + (t - 1)) * c.subGroups + (s - 1);
}

// Alle drei Anzahlen müssen mindestens 1 sein
inline bool config_valid(const GroupConfig &c) {
    return c.mainGroups > 0 && c.tasks > 0 && c.subGroups > 0;
}

// Prüft, ob (m,t,s) innerhalb der in der CSV festgelegten Anzahlen liegt
inline bool group_in_range(const GroupConfig &c, uint8_t m, uint8_t t, uint8_t s) {
    return m >= 1 && m <= c.mainGroups &&
           t >= 1 && t <= c.tasks &&
           s >= 1 && s <= c.subGroups;
}

// Interner ESP32 DAC - 8-bit Auflösung (PWM mit 0 bis 255 Stufen)
static constexpr uint8_t  DAC_RESOLUTION      = 255;

// Interner ESP32 DAC - Spannungsbereich in mV
static constexpr float    DAC_VOLTAGE_MIN_mV  = 0.0f; 
static constexpr float    DAC_VOLTAGE_MAX_mV  = 3000.0f;

// Interne DAC Pins des ESP32 (GPIO25 und GPIO26  DAC Pins)
static constexpr uint8_t  DAC_PIN_CH1         = 25;  // DAC1 -> Kanal 1
static constexpr uint8_t  DAC_PIN_CH2         = 26;  // DAC2 -> Kanal 2

