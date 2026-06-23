/**
 * @file save_data.cpp
 * @brief Umsetzung für das Speichern der Gruppen im NVS (Non-Volatile Storage)
 * Sobald eine Datei neu geladen wird, löscht sich der alte Speicher und 
 * im NVS werden die neuen Informationen abgelegt.
 */
//----------------------------------------------------------------------
#include "save_data.h"
#include <Preferences.h>

static Preferences prefs;                  // NVS Instanz
static const char* NVS_NAMESPACE = "dac";  // Namespace im NVS
static const char* KEY_COUNT = "count";    // Schlüssel für die Anzahl der Gruppen

/**
 * Initialisiert den NVS Speicher
 * false = read/write Modus
 */
bool nvs_init() {
  return prefs.begin(NVS_NAMESPACE, false);
}

/**
 * Speichert alle Gruppen im NVS.
 * Löscht zuerst den alten Speicher und schreibt dann die neuen Gruppen.
 * Schlüssel pro Gruppe: "g01" bis "g30"
 */
void nvs_save_all(const SignalGroup* groups, uint8_t amount) {
  prefs.clear();  // Alten Speicher löschen

  for (uint8_t i = 0; i < amount; i++) {
    char key[8];
    snprintf(key, sizeof(key), "g%02u", groups[i].group_id);  // Schlüssel z.B. "g01"
    prefs.putBytes(key, &groups[i], sizeof(SignalGroup));     // Gruppe speichern
  }
  prefs.putUChar(KEY_COUNT, amount);  // Anzahl der Gruppen speichern

  Serial.printf("NVS: %u Gruppen gespeichert\n", amount);
}

/**
 * Lädt alle gespeicherten Gruppen aus dem NVS.
 * Gibt die Anzahl der geladenen Gruppen zurück.
 */
uint8_t nvs_load_all(SignalGroup* groups_out) {
  uint8_t amount = prefs.getUChar(KEY_COUNT, 0);  // Anzahl der gespeicherten Gruppen lesen

  if (amount == 0) {
    Serial.println("NVS: keine gespeicherten Gruppen");
    return 0;
  }

  for (uint8_t i = 0; i < amount; i++) {
    char key[8];
    snprintf(key, sizeof(key), "g%02u", i + 1);  // group_id startet bei 1

    size_t len = prefs.getBytes(key, &groups_out[i], sizeof(SignalGroup));  // Gruppe laden
    if (len != sizeof(SignalGroup)) {
      Serial.printf("NVS: Fehler beim Lesen von %s\n", key);
    }
  }

  Serial.printf("NVS: %u Gruppen geladen\n", amount);
  return amount;
}

/**
 * Lädt eine einzelne Gruppe aus dem NVS anhand der Gruppen-ID.
 * Gibt true zurück wenn die Gruppe erfolgreich geladen wurde.
 */
bool nvs_load_group(uint8_t group_id, SignalGroup& out) {
  char key[8];
  snprintf(key, sizeof(key), "g%02u", group_id);                // Schlüssel z.B. "g01"
  size_t len = prefs.getBytes(key, &out, sizeof(SignalGroup));  // Gruppe laden
  return len == sizeof(SignalGroup);                            // true wenn vollständig geladen
}