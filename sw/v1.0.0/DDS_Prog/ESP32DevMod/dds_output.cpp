/**
 * @file dds_output.cpp
 * @brief Datei mit Funktionen zur Ansteuerung der DDS-Ausgänge basierend auf den Signaleinstellungen der Signalgruppen.
 */

//------------------------------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include "AD9833.h"
#include "signal_config.h"
#include "MCP42010.h"
#include "dds_output.h"
#include "resistance.h"
#include "ws2812b.h"

//------------------------------------------------------------------------------------------

/**
 * Funktion zur Umrechnung von Millivolt in Volt
 */
float mv_to_v(float mV){
    return mV / 1000.0f;
}

/**
 * Funktion zum Starten der DDS-Ausgabe basierend auf den Signaleinstellungen der übergebenen Signalgruppen.
 */
void dds_start(const SignalGroup* gruppen, uint8_t anzahl) {
    
    const SignalGroup& g = gruppen[0];

    // Phase beider ICs auf 0 zurücksetzten
    UpdatePhase(0, 0);
    UpdatePhase(0, 1);

    // Betriebsmodus der beiden Kanäle festlegen (Sinus oder Rechteck)
    mode[0] = g.ch1.signalType;
    mode[1] = g.ch2.signalType;
   
    // Frequenz setzen
    if (g.ch1.frequency_Hz >= 0.0f && g.ch1.frequency_Hz <= float(MAX_FREQ)){
        f[0] = g.ch1.frequency_Hz;
    }
    
    if (g.ch2.frequency_Hz >= 0.0f && g.ch2.frequency_Hz <= float(MAX_FREQ)){
        f[1] = g.ch2.frequency_Hz;
    }
    
    if (f[0]==f[1]){
        SyncChips();
    }
    else {
        UpdateFreq(f[0], 0);
        UpdateFreq(f[1], 1);
    }

    // Ausgangsspannung einstellen
    if (mv_to_v(g.ch1.amplitude_mV) >= 0.0f) {
        calculateResistance(mv_to_v(g.ch1.amplitude_mV), 0);
    }
    if (mv_to_v(g.ch2.amplitude_mV) >= 0.0f) {
        calculateResistance(mv_to_v(g.ch2.amplitude_mV), 1);
    }

    //Pahse setzen
    if (g.ch1.phase_deg <= 360.0f && g.ch1.phase_deg >= 0.0f) {
        phase[0] = g.ch1.phase_deg;
        UpdatePhase(phase[0], 0);
    }
    if (g.ch2.phase_deg <= 360.0f && g.ch2.phase_deg >= 0.0f) {
        phase[1] = g.ch2.phase_deg;
        UpdatePhase(phase[1], 1);
    }

    // Sleep Mode von IC1 deaktivieren
    UpdateRegister(activeControl[0] &= ~(SLEEP | RESET), 0);

    // Sleep Mode von IC2 deaktivieren
    UpdateRegister(activeControl[1] &= ~(SLEEP | RESET), 1);

    // Led-Status aktualisieren 
    dds_activated();

    // Augabe der Information im Serial Monitor
    Serial.printf("DDS start | G%u | CH1: %.1fmV %.1fHz %.1f° | CH2: %.1fmV %.1fHz %.1f° | Signalform CH1: %hhu\n Signalform CH2: %hhu\n",
        g.group_id,
        g.ch1.amplitude_mV, g.ch1.frequency_Hz, g.ch1.phase_deg,
        g.ch2.amplitude_mV, g.ch2.frequency_Hz, g.ch2.phase_deg, g.ch1.signalType, g.ch2.signalType);
}

/**
 * Funktion zum Stoppen der DDS-Ausgabe
 */
void dds_stop() {
    // ICs zurücksetzten
    UpdateRegister(activeControl[1] |= RESET, 1);
    delayMicroseconds(2);
    // Sleep Mode aktivieren
    UpdateRegister(activeControl[1] |= SLEEP, 1);
    

    // ICs zurücksetzten
    UpdateRegister(activeControl[0] |= RESET, 0);
    delayMicroseconds(2);
    // Sleep Mode aktivieren
    UpdateRegister(activeControl[0] |= SLEEP, 0);

    //Led-Status aktualisieren
    dds_deactivated();
}