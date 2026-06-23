/**
 * @file csv.cpp
 * @brief Implementierung der Filtrierung von den Parametern in der CSV Datei.
 * Die Parameter der CSV-Datei sollten im folgenden Format vorliegen und GruppenDAC.csv benannt sein:
 * Gruppe, Amplitude CH1, Frequenz CH1, Phase CH1, Signal CH1, Amplitude CH2, Frequenz CH2, Phase CH2, Signal CH2
 *   1,  410,  10,   81,  1,  830,  10,  11,  1
 *   2,  830,  45,   14,  0,  1650, 45,  27,  0
 *   3,  240,  56,   3,   1,  620,  56,  29,  0
 *   4,  1500, 83,   35,  0,  1030, 83,  64,  1
 * 
 * Das Einlesen erfolgt erst nach der Eingabe des Nutzers
 */
//----------------------------------------------------------------------
#include "csv.h"

// SPI Bus Instanz für die SD-Karte (HSPI)
static SPIClass sd_spi(HSPI);

/**
 * Liest den nächsten Wert aus einer CSV-Zeile bis zum nächsten Komma oder Zeilenende.
 * Verschiebt den Zeiger auf das nächste Feld.
 */
static void csv_read_value(const char *&pointer, char *buffer) {
    uint8_t i = 0;

    // Führende Leerzeichen überspringen
    while (*pointer == ' ') pointer++;

    // Zeichen lesen bis Komma, Zeilenende oder Stringende
    while (*pointer != ',' && *pointer != '\n' && *pointer != '\r' && *pointer != '\0') {
        buffer[i++] = *pointer++;
    }
    buffer[i] = '\0';  // String abschließen

    // Komma überspringen falls vorhanden
    if (*pointer == ',') pointer++;
}

/**
 * Liest eine komplette Zeile von der SD-Karte in den Puffer.
 * Gibt die Länge der gelesenen Zeile zurück.
 */
static uint8_t csv_read_line(File &file, char *buffer, size_t max_len) {
    uint8_t len = 0;
    memset(buffer, 0, max_len);  // Puffer leeren

    while (file.available()) {
        char c = file.read();
        if (c == '\n') break;        // Zeilenende → fertig
        if (c == '\r') continue;     // Carriage Return überspringen (Windows-Zeilenenden)
        if (len < max_len - 1) buffer[len++] = c;  // Zeichen in Puffer schreiben
    }
    return len;
}

/**
 * Parst eine CSV-Zeile und befüllt eine SignalGroup.
 * Reihenfolge: Gruppe, A1, F1, P1, S1, A2, F2, P2, S2
 */
static void csv_parse_line(const char *line_buffer, SignalGroup &group) {
    const char *pointer = line_buffer;
    char value_buffer[32];

    memset(&group, 0, sizeof(group));  // Gruppe mit 0 initialisieren

    // Spalte 1: Gruppen-ID
    csv_read_value(pointer, value_buffer);
    group.group_id = (uint8_t)atoi(value_buffer);

    // CH1 Parameter — Spalte 2 bis 5
    csv_read_value(pointer, value_buffer);
    group.ch1.amplitude_mV = atof(value_buffer);   // Amplitude in mV

    csv_read_value(pointer, value_buffer);
    group.ch1.frequency_Hz = atof(value_buffer);   // Frequenz in Hz

    csv_read_value(pointer, value_buffer);
    group.ch1.phase_deg = atof(value_buffer);       // Phasenwinkel in Grad

    csv_read_value(pointer, value_buffer);
    group.ch1.signalType = atoi(value_buffer);      // Signalform (0=Sinus, 1=Dreieck, ...)

    // CH2 Parameter — Spalte 6 bis 9
    csv_read_value(pointer, value_buffer);
    group.ch2.amplitude_mV = atof(value_buffer);   // Amplitude in mV

    csv_read_value(pointer, value_buffer);
    group.ch2.frequency_Hz = atof(value_buffer);   // Frequenz in Hz

    csv_read_value(pointer, value_buffer);
    group.ch2.phase_deg = atof(value_buffer);       // Phasenwinkel in Grad

    csv_read_value(pointer, value_buffer);
    group.ch2.signalType = atoi(value_buffer);      // Signalform (0=Sinus, 1=Dreieck, ...)
}

/**
 * Initialisiert die SD-Karte und den SPI-Bus.
 * Wird erst durch Betätigen von "Daten Laden" auf dem HMI aufgerufen.
 */
bool csv_init(uint8_t cs_pin) {
    sd_spi.begin(14, 16, 17, cs_pin);  // SCLK, MISO, MOSI, CS

    if (!SD.begin(cs_pin, sd_spi)) {
        Serial.println("Fehler: SD-Karte konnte nicht initialisiert werden.");
        return false;
    }
    Serial.println("SD-Karte erfolgreich initialisiert.");
    return true;
}

/**
 * Liest alle Gruppen aus der CSV-Datei und speichert sie im Array.
 * Gibt die Anzahl der gelesenen Gruppen über group_count zurück.
 */
bool csv_read(SignalGroup groups_array[], uint8_t &group_count) {
    group_count = 0;

    // Datei öffnen
    File file = SD.open(CSV_FILENAME, FILE_READ);
    if (!file) {
        Serial.println("Fehler: Datei konnte nicht geöffnet werden.");
        return false;
    }

    char line_buffer[256];
    bool header_skipped = false;

    // Zeilenweise lesen bis MAX_GROUPS erreicht
    while (group_count < MAX_GROUPS) {

        uint8_t line_len = csv_read_line(file, line_buffer, sizeof(line_buffer));

        // Leere Zeile — prüfen ob Dateiende
        if (line_len == 0) {
            if (!file.available()) break;
            continue;
        }

        // Erste Zeile ist der Header → überspringen
        if (!header_skipped) {
            header_skipped = true;
            if (!file.available()) break;
            continue;
        }

        // Zeile parsen und in Array speichern
        SignalGroup group;
        csv_parse_line(line_buffer, group);
        groups_array[group_count++] = group;

        // Dateiende prüfen
        if (!file.available()) break;
    }

    file.close();

    Serial.print("Erfolgreich eingelesen: ");
    Serial.print(group_count);
    Serial.println(" Gruppen.");

    return true;
}

/**
 * Gibt alle Parameter einer Gruppe auf dem Serial Monitor aus (Debugging)
 */
void csv_debug(const SignalGroup &group) {
    Serial.print("Gruppe: ");
    Serial.println(group.group_id);

    Serial.print(" CH1 -> Amp: ");   Serial.print(group.ch1.amplitude_mV);
    Serial.print(" mV | Freq: ");    Serial.print(group.ch1.frequency_Hz);
    Serial.print(" Hz | Phase: ");   Serial.print(group.ch1.phase_deg);
    Serial.print(" deg | Signalform: "); Serial.println(group.ch1.signalType);

    Serial.print(" CH2 -> Amp: ");   Serial.print(group.ch2.amplitude_mV);
    Serial.print(" mV | Freq: ");    Serial.print(group.ch2.frequency_Hz);
    Serial.print(" Hz | Phase: ");   Serial.print(group.ch2.phase_deg);
    Serial.print(" deg | Signalform: "); Serial.println(group.ch2.signalType);
}