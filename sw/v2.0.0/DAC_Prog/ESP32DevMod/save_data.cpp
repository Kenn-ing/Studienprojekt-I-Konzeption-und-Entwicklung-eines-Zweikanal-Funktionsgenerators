/**
 * @file save_data.cpp
 * @brief Umsetzung für das Speichern der Gruppen im NVS (Non-Volatile Storage)
 * Sobald eine Datei neu geladen wird, löscht sich der alte Speicher und 
 * im NVS werden die neuen Informationen abgelegt.
 */
//----------------------------------------------------------------------
#include "save_data.h"
#include <Preferences.h>

static Preferences prefs;                          // NVS Instanz
static const char* NVS_NAMESPACE = "dac";          // Namespace im NVS
static const char* KEY_COUNT     = "count";        // Schlüssel für die Anzahl der Gruppen
static const char* KEY_MAIN  = "mg";               // Schlüssel für die Anzahl der Hauptgruppen
static const char* KEY_TASKS = "t";                // Schlüssel für die Anzahl der Aufgaben
static const char* KEY_SUB   = "sg";               // Schlüssel für die Anzahl der Untergruppen

/**
 * Initialisiert den NVS Speicher
 * false = read/write Modus
 */
bool nvs_init() {
    return prefs.begin(NVS_NAMESPACE, false);
}

/**
 * Speichert alle Gruppen und die Anzahlen im NVS.
 * Löscht zuerst den alten Speicher und schreibt dann die neuen Gruppen.
 * Schlüssel pro Gruppe: "g0", "g1", "g2", ... (Position im Array, also group_index von Hauptgruppe/Aufgabe/Untergruppe)
 * Die Anzahlen und der Zähler werden ganz zum Schluss geschrieben. Bricht das Speichern vorher ab,
 * fehlt der Zähler und beim Laden gilt der Speicher als ungültig.
 * Gibt true zurück wenn alles geschrieben werden konnte.
 */
bool nvs_save_all(const SignalGroup* groups, const GroupConfig& cfg, uint32_t amount) {
    prefs.clear();  // Alten Speicher löschen

    for (uint32_t i = 0; i < amount; i++) {
        char key[16];
        snprintf(key, sizeof(key), "g%lu", (unsigned long)i);       // Schlüssel z.B. "g0"

        // Schreiben prüfen: bei vollem NVS liefert putBytes weniger als sizeof(SignalGroup) zurück
        if (prefs.putBytes(key, &groups[i], sizeof(SignalGroup)) != sizeof(SignalGroup)) {
            Serial.printf("NVS: Fehler beim Schreiben von %s (Speicher voll?)\n", key);
            prefs.clear();                                          // halbfertigen Stand nicht liegen lassen
            return false;
        }
    }

    prefs.putUChar(KEY_MAIN,  cfg.mainGroups);   // Anzahlen speichern
    prefs.putUChar(KEY_TASKS, cfg.tasks);
    prefs.putUChar(KEY_SUB,   cfg.subGroups);
    prefs.putUInt(KEY_COUNT, amount);            // Anzahl der Gruppen speichern (zuletzt!)

    Serial.printf("NVS: %lu Gruppen gespeichert\n", (unsigned long)amount);
    return true;
}

/**
 * Lädt die gespeicherten Anzahlen (Hauptgruppen, Aufgaben, Untergruppen) aus dem NVS.
 * Gibt true zurück wenn sie vorhanden sind und zur gespeicherten Gruppenanzahl passen.
 */
bool nvs_load_config(GroupConfig& cfg) {
    if (!prefs.isKey(KEY_COUNT) || !prefs.isKey(KEY_MAIN) ||
        !prefs.isKey(KEY_TASKS) || !prefs.isKey(KEY_SUB)) {
        Serial.println("NVS: keine gespeicherten Gruppen");
        return false;
    }

    cfg.mainGroups = prefs.getUChar(KEY_MAIN, 0);
    cfg.tasks      = prefs.getUChar(KEY_TASKS, 0);
    cfg.subGroups  = prefs.getUChar(KEY_SUB, 0);
    uint32_t count = prefs.getUInt(KEY_COUNT, 0);

    // M*T*S muss mit der gespeicherten Gruppenanzahl übereinstimmen
    return config_valid(cfg) && count == group_total(cfg);
}

/**
 * Lädt eine einzelne Gruppe aus dem NVS anhand von Hauptgruppe, Aufgabe und Untergruppe.
 * Gibt true zurück wenn die Gruppe erfolgreich geladen wurde.
 */
bool nvs_load_group(const GroupConfig& cfg, uint8_t m, uint8_t t, uint8_t s, SignalGroup& out) {
    if (!group_in_range(cfg, m, t, s)) return false;                // Kombination gibt es gar nicht

    char key[16];
    snprintf(key, sizeof(key), "g%lu", (unsigned long)group_index(cfg, m, t, s));  // Schlüssel z.B. "g5"
    size_t len = prefs.getBytes(key, &out, sizeof(SignalGroup));    // Gruppe laden

    // vollständig gelesen und die Indizes in der Gruppe passen zur Anfrage?
    return len == sizeof(SignalGroup) &&
           out.main_group == m && out.task == t && out.sub_group == s;
}