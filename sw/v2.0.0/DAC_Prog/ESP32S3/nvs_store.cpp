/**
 * @file nvs_store.cpp
 * @brief KOMPLETT NEU (Master): Anzahlen im NVS ablegen (Library "Preferences").
 * Die Gruppendaten selbst werden auf dem Master NICHT gespeichert (sie liegen auf der SD-Karte
 * und werden nach dem Senden aus dem RAM freigegeben). Der Slave speichert sie in seinem eigenen NVS.
 */
#include "nvs_store.h"
#include <Preferences.h>

static const char *NS = "sigcfg";   // Namespace im NVS

/**
 * Speichert die Anzahlen. Es wird nur geschrieben, wenn sich ein Wert geändert hat,
 * um den Flash nicht unnötig abzunutzen.
 */
void nvs_save_config(const GroupConfig &c) {
    Preferences p;
    if (!p.begin(NS, false)) return;                       // false = Lese- und Schreibzugriff
    if (p.getUChar("mg", 0) != c.mainGroups) p.putUChar("mg", c.mainGroups);
    if (p.getUChar("t",  0) != c.tasks)      p.putUChar("t",  c.tasks);
    if (p.getUChar("sg", 0) != c.subGroups)  p.putUChar("sg", c.subGroups);
    p.end();
}

/**
 * Lädt die Anzahlen. Wird beim Start aufgerufen, damit die Dropdowns auch ohne
 * SD-Karte/CSV die richtigen Auswahlmöglichkeiten haben.
 */
bool nvs_load_config(GroupConfig &c) {
    Preferences p;
    if (!p.begin(NS, true)) return false;                  // true = nur lesen; false, wenn Namespace noch nicht existiert
    bool ok = p.isKey("mg") && p.isKey("t") && p.isKey("sg");
    if (ok) {
        c.mainGroups = p.getUChar("mg", 0);
        c.tasks      = p.getUChar("t",  0);
        c.subGroups  = p.getUChar("sg", 0);
    }
    p.end();
    return ok && config_valid(c);
}