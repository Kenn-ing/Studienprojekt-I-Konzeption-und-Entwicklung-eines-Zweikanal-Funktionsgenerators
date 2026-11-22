/**
 * @file resistance.cpp
 * @brief Datei für die Berechnung des Widerstands basierend auf der vorgegebenen Spannung des Benutzers.
 * Diese Datei implementiert die Funktion calculateResistance, die die benötigten Widerstandswerte für die Potentiometer berechnet, um die gewünschte Ausgangsspannung zu erreichen, basierend auf der aktuellen Frequenz und dem Modus des AD9833 Chips.
 */
//--------------------------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include "resistance.h"
#include "MCP42010.h"
#include "AD9833.h"
//--------------------------------------------------------------------------------------

//Lookup-Tabellen für die Berechnung der benötigten Widerstände basierend auf der Frequenz des AD9833 Chips im Sinusmodus (Ausgangsspannung des AD9833 ist im Sinusmodus frequenzabhängig).
static const float freqTable[] = {1.0f,         10.0f,          100.0f,         1000.0f,        10000.0f,       100000.0f,      500000.0f,      700000.0f,      1000000.0f,     1500000.0f,     1800000.0f,     2000000.0f};
static const float voltTable[] = {0.287,       0.298f,         0.298f,         0.298f,         0.298,         0.298f,         0.298f,         0.298f,         0.298f,         0.298f,         0.273f,         0.273f};
static const int tableSize = 12;

// Konstante Ausgangsspannung im Rechteckmodus (weitestgehend unabhängig von der Frequenz)
static const float voltSqu = 1.65f;


/**
 * Funktion zum Ausgeben der Verstärkung des OPVs basierend auf der Chip-Nummer (0 oder 1).
 */
float getVoltAmplification(int chip){
    if(chip==0){
        return 37.2;
    }
    if(chip==1){
        return 37.5;
    }
}

/**
 * Funktion zum Berechnen der Eingangsspannung der Potentiometer basierend auf der aktuellen Frequenz und dem Modus des AD9833 Chips.
 * Im Sinusmodus ist die Ausgangsspannung des AD9833 frequenzabhängig, weshalb eine Interpolation in der freqTable/voltTable durchgeführt wird. Im Rechteckmodus wird eine konstante Spannung angenommen.
 */
float getInputVoltage(float freqHz, int chip){

    // Überprüfen, in welchem Modus der AD9833 betrieben wird. Wird ein Rechtecksignal ausgegeben, wird die konstante Spannung zurückgegeben.
    if (mode[chip]==1){
        return voltSqu;
    }
    // Im Sinusmodus wird die Spannung anhand der Frequenz interpoliert. Für Frequenzen außerhalb der Tabelle werden die Grenzwerte zurückgegeben.
    else {
        // Grenzfälle: Wenn die Frequenz außerhalb der Tabelle liegt, den jeweils nächsten Wert zurückgeben
        if (freqHz <= freqTable[0]) return voltTable[0];
        if (freqHz >= freqTable[tableSize-1]) return voltTable[tableSize-1];

        // Interpolation: Den Bereich in der Tabelle finden, in dem die Frequenz liegt, und dann logarithmisch interpolieren
        for (int i = 0; i < tableSize - 1; i++) {
            if (freqHz >= freqTable[i] && freqHz < freqTable[i + 1]) {
              // Logarithmische Position zwischen den zwei Stützpunkten berechnen
              float t = log(freqHz / freqTable[i]) / log(freqTable[i+1] / freqTable[i]);
              return voltTable[i] + t * (voltTable[i+1] - voltTable[i]);
            }
        }
        return voltTable[tableSize - 1];
    }
}

/**
 * Funktion zum Berechnen der Widerstände für die Potentiometer basierend auf der Zielspannung, der aktuellen Frequenz und dem Modus des AD9833 Chips.
 * Die Funktion berechnet die benötigten Widerstandswerte für P0 und P1, um die gewünschte Ausgangsspannung zu erreichen, und setzt die Potentiometer entsprechend. 
 */
void calculateResistance(float targetVoltage, int chip) {
    // Abfrage der aktuellen Eingangsspannung der Potentiometer basierend auf der Frequenz und dem Modus des AD9833 Chips
    float inputVoltage = getInputVoltage(f[chip], chip);

    // Abfrage der Verstärkung des OPVs basierend auf der Chip-Nummer
    float voltAmplification = getVoltAmplification(chip);

    // Berechnung der maximalen und minimalen Ausgangsspannung, die mit den Potentiometern erreicht werden kann, um die Zielspannung auf Plausibilität zu prüfen
    float maxVout = (P0_255[chip] / (P0_255[chip] + P1_0[chip])) * inputVoltage * voltAmplification;
    float minVout = (P0_0[chip]  / (P0_0[chip]  + P1_255[chip])) * inputVoltage * voltAmplification;

    // Wenn die Zielspannung außerhalb des Bereichs liegt, der mit den Potentiometern erreicht werden kann, eine Fehlermeldung ausgeben und die Funktion verlassen
    if (targetVoltage > maxVout) {
        Serial.printf("Fehler: Maximum ist %.3f V\n", maxVout);
        return;
    }
    if (targetVoltage < minVout) {
        Serial.printf("Fehler: Minimum ist %.3f V\n", minVout);
        return;
    }

    // Potentiometer 0 auf 255 setzen (maximaler Widerstand), um die benötigte Position von Potentiometer 1 zu berechnen.
    int newPot0 = 255;

    // Division der Zielspannung durch die Verstärkung des OPVs, um die benötigte Eingangsspannung zu berechnen.
    float realTarget = targetVoltage / voltAmplification;
    
    float ratio = (inputVoltage-realTarget)/realTarget; // Verhältnis P1/P0, das benötigt wird, um die Zielspannung zu erreichen
    
    float targetP1kOhm = ratio * P0_255[chip]; // Berechnung des benötigten Widerstands für P1, um die Zielspannung zu erreichen

    // P1 Position aus Widerstandswert berechnen: pos = (R - P1_0) / (P1_255 - P1_0) * 255
    int newPot1 = (int)((targetP1kOhm - P1_0[chip]) / (P1_255[chip] - P1_0[chip]) * 255.0f + 0.5f); //+0.5f für korrektes Runden zum nächsten Integer
    newPot1 = constrain(newPot1, 0, 255); // Sicherstellen, dass der Wert im gültigen Bereich liegt

    // Falls P1 durch constrain begrenzt wurde, P0 nachrechnen
    if (newPot1 == 255) {
        float actualP1_kOhm = P1_255[chip];
        float targetP0kOhm = actualP1_kOhm / ratio;
        newPot0 = (int)((targetP0kOhm - P0_0[chip]) / (P0_255[chip] - P0_0[chip]) * 255.0f + 0.5f);
        newPot0 = constrain(newPot0, 1, 255); // mind. 1, damit nicht mit 0 dividiert wird
    }

    // Tatsächliche Ausgangsspannung berechnen und ausgeben
    float actualP0_kOhm = (newPot0 / 255.0f * (P0_255[chip] - P0_0[chip])) + P0_0[chip];
    float actualP1_kOhm = (newPot1 / 255.0f * (P1_255[chip] - P1_0[chip])) + P1_0[chip];
    float actualVout = inputVoltage * (actualP0_kOhm / (actualP0_kOhm + actualP1_kOhm)) * voltAmplification;

    Serial.println("─────────────────────────────────────");
    Serial.printf("  Zielspannung:      %.3f V\n", targetVoltage);
    Serial.printf("  Erreichte Spannung: %.3f V\n", actualVout);
    Serial.printf("  Abweichung:        %.3f V\n", actualVout - targetVoltage);
    Serial.printf("  Pot0 für IC %d: %3d/255  (~%.3f kΩ)\n", chip, newPot0, actualP0_kOhm);
    Serial.printf("  Pot1 für IC %d: %3d/255  (~%.3f kΩ)\n", chip, newPot1, actualP1_kOhm);
    Serial.println("─────────────────────────────────────");

    setPot0(newPot0, chip);
    setPot1(newPot1, chip);
}