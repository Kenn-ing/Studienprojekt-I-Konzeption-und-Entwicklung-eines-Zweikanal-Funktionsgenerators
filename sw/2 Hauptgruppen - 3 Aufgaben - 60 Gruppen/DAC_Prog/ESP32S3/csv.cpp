/**
 * @file csv.cpp
 * @brief Implementierung der Filtrierung von den Parametern in der CSV Datei.
 * Die Parameter der CSV-Datei sollten im folgenden Format vorliegen und GruppenDAC.csv benannt sein:
 * 
 * Gesamtzahl Hauptgruppen, Gesamtzahl Aufgaben, Gesamtzahl Untergruppen
 * 2, 2, 1
 * Hauptgruppe, Aufgabe, Untergruppe, Amplitude CH1, Frequenz CH1, Phase CH1, Signal CH1, Amplitude CH2, Frequenz CH2, Phase CH2, Signal CH2
 *   1, 1, 1,  410,  10,   81,  1,  830,  10,  11,  1
 *   1, 2, 1,  830,  45,   14,  0,  1650, 45,  27,  0
 *   2, 1, 1,  240,  56,   3,   1,  620,  56,  29,  0
 *   2, 2, 1,  1500, 83,   35,  0,  1030, 83,  64,  1
 * 
 * Das Einlesen erfolgt erst nach der Eingabe des Nutzers
 */
//----------------------------------------------------------------------
#include "csv.h"

// SPI Bus Instanz für die SD-Karte (HSPI)
static SPIClass sd_spi(HSPI);

/**
 * Liest den nächsten Wert aus einer CSV-Zeile bis zum nächsten Komma oder Zeilenende.
 * Verschiebt den Zeiger auf das nächste Feld. Überlange Felder werden abgeschnitten.
 * buffer muss mindestens 32 Byte groß sein.
 */
static void csv_read_value(const char *&pointer, char *buffer) {
    uint8_t i = 0;

    // Führende Leerzeichen überspringen
    while (*pointer == ' ') pointer++;

    // Zeichen lesen bis Komma, Zeilenende oder Stringende
    while (*pointer != ',' && *pointer != '\n' && *pointer != '\r' && *pointer != '\0') {
        if (i < 31) buffer[i++] = *pointer;   // nur schreiben solange Platz ist
        pointer++;
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
 * Parst eine Datenzeile und befüllt eine SignalGroup.
 * Reihenfolge: Hauptgruppe, Aufgabe, Untergruppe, A1, F1, P1, S1, A2, F2, P2, S2
 * (vorher: nur eine Gruppennummer statt der drei Indizes)
 */
static void csv_parse_line(const char *line_buffer, SignalGroup &group) {
    const char *pointer = line_buffer;
    char value_buffer[32];

    memset(&group, 0, sizeof(group));  // Gruppe mit 0 initialisieren

    // Spalten 1-3: Hauptgruppe, Aufgabe, Untergruppe
    csv_read_value(pointer, value_buffer);
    group.main_group = (uint8_t)atoi(value_buffer);

    csv_read_value(pointer, value_buffer);
    group.task = (uint8_t)atoi(value_buffer);

    csv_read_value(pointer, value_buffer);
    group.sub_group = (uint8_t)atoi(value_buffer);



    // CH1 Parameter — Spalte 4 bis 7
    csv_read_value(pointer, value_buffer);
    group.ch1.amplitude_mV = atof(value_buffer);   // Amplitude in mV

    csv_read_value(pointer, value_buffer);
    group.ch1.frequency_Hz = atof(value_buffer);   // Frequenz in Hz

    csv_read_value(pointer, value_buffer);
    group.ch1.phase_deg = atof(value_buffer);       // Phasenwinkel in Grad

    csv_read_value(pointer, value_buffer);
    group.ch1.signalType = atoi(value_buffer);      // Signalform (0=Sinus, 1=Dreieck, ...)



    // CH2 Parameter — Spalte 8 bis 11
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
 * Liest die CSV-Datei in drei Abschnitten (gezählt werden nur nicht-leere Zeilen):
 *   Zeile 1: Beschriftung der Anzahlen          -> überspringen
 *   Zeile 2: Anzahlen M, T, S                   -> cfg füllen, group_count = M*T*S berechnen, Array in genau dieser Größe anlegen
 *   Zeile 3: Beschriftung der Datentabelle      -> überspringen
 *   ab Zeile 4: Datenzeilen                     -> anhand (m,t,s) an die richtige Position schreiben
 *
 * Zeilen mit Indizes außerhalb der Anzahlen werden ignoriert. Ein Platz gilt als belegt,
 * wenn main_group != 0 ist.
 * Am Ende muss jede Kombination genau einmal belegt worden sein (filled == group_count).
 * Wenn nicht, wird ein Fehler zurückgegeben.
 */
bool csv_read(SignalGroup *&groups, GroupConfig &cfg, uint32_t &group_count) {
    // Altes Array aus einem früheren Aufruf freigeben
    free(groups);
    groups = nullptr;
    group_count = 0;

    File file = SD.open(CSV_FILENAME, FILE_READ);
    if (!file) {
        Serial.println("Fehler: Datei konnte nicht geöffnet werden.");
        return false;
    }

    char line_buffer[256];
    uint8_t line_no = 0;      // Nummer der aktuellen nicht leeren Zeile
    uint32_t filled = 0;      // Anzahl bisher belegter (unterschiedlicher) Plätze

    while (file.available()) {
        // Leere Zeilen ignorieren
        if (csv_read_line(file, line_buffer, sizeof(line_buffer)) == 0) continue;
        line_no++;

        if (line_no == 1) continue;                  // Beschriftung der Anzahlen

        if (line_no == 2) {                          // Anzahlen einlesen
            const char *pointer = line_buffer;
            char value_buffer[32];
            csv_read_value(pointer, value_buffer);  cfg.mainGroups = (uint8_t)atoi(value_buffer);
            csv_read_value(pointer, value_buffer);  cfg.tasks      = (uint8_t)atoi(value_buffer);
            csv_read_value(pointer, value_buffer);  cfg.subGroups  = (uint8_t)atoi(value_buffer);

            if (!config_valid(cfg)) {
                Serial.println("Fehler: Anzahlen in der CSV ungueltig.");
                file.close();
                return false;
            }

            group_count = group_total(cfg);          // N = M * T * S  (ersetzt MAX_GROUPS)
            groups = (SignalGroup *)calloc(group_count, sizeof(SignalGroup));
            if (!groups) {
                Serial.println("Fehler: Nicht genug Speicher fuer die Gruppen.");
                group_count = 0;
                file.close();
                return false;
            }
            continue;
        }

        if (line_no == 3) continue;                  // Beschriftung der Datentabelle

        // Datenzeile parsen
        SignalGroup g;
        csv_parse_line(line_buffer, g);

        // Indizes außerhalb der Anzahlen  werden ignorieren
        if (!group_in_range(cfg, g.main_group, g.task, g.sub_group)) continue;

        SignalGroup &slot = groups[group_index(cfg, g.main_group, g.task, g.sub_group)];
        if (slot.main_group == 0) filled++;          // Platz war noch leer -> neue Kombination
        slot = g;                                    // (doppelte Zeile überschreibt, zählt aber nicht doppelt)
    }
    file.close();

    // Vollständigkeitsprüfung: Header muss da gewesen sein und jede Kombination genau einmal
    if (!groups || filled != group_count) {
        Serial.printf("Fehler: CSV unvollstaendig (%lu von %lu Gruppen).\n",
                      (unsigned long)filled, (unsigned long)group_count);
        free(groups);
        groups = nullptr;
        group_count = 0;
        return false;
    }

    Serial.printf("Erfolgreich eingelesen: %lu Gruppen (%u Hauptgruppen, %u Aufgaben, %u Untergruppen).\n",
                  (unsigned long)group_count, cfg.mainGroups, cfg.tasks, cfg.subGroups);
    return true;
}

/**
 * Gibt alle Parameter einer Gruppe auf dem Serial Monitor aus (Debugging)
 */
void csv_debug(const SignalGroup &group) {
    Serial.printf("Hauptgruppe: %u | Aufgabe: %u | Untergruppe: %u\n",
                  group.main_group, group.task, group.sub_group);

    Serial.print(" CH1 -> Amp: ");   Serial.print(group.ch1.amplitude_mV);
    Serial.print(" mV | Freq: ");    Serial.print(group.ch1.frequency_Hz);
    Serial.print(" Hz | Phase: ");   Serial.print(group.ch1.phase_deg);
    Serial.print(" deg | Signalform: "); Serial.println(group.ch1.signalType);

    Serial.print(" CH2 -> Amp: ");   Serial.print(group.ch2.amplitude_mV);
    Serial.print(" mV | Freq: ");    Serial.print(group.ch2.frequency_Hz);
    Serial.print(" Hz | Phase: ");   Serial.print(group.ch2.phase_deg);
    Serial.print(" deg | Signalform: "); Serial.println(group.ch2.signalType);
}