/**
 * @file ui_config.cpp
 * @brief KOMPLETT NEU (Master): Dropdown-Optionen aus den Anzahlen erzeugen.
 *
 * Die Dropdowns in screens.c holen ihre Optionen über die EEZ-Variablen
 * (get_var_main_group_text / get_var_question_text / get_var_sub_group_text) und setzen sie im
 * tick-Code selbständig per lv_dropdown_set_options. Wir müssen also nur die Variablen setzen.
 *
 * ACHTUNG: In screens.c sind die Dropdowns über Kreuz verdrahtet:
 *   droppdown_sub_group -> question_text / active_question
 *   droppdown_question  -> sub_group_text / active_sub_group
 * Hier wird nach den VARIABLEN gefüllt: question_text bekommt die Anzahl der Aufgaben,
 * sub_group_text die der Untergruppen. Prüfe in EEZ Studio, welches Dropdown optisch welches ist.
 */
#include "ui_config.h"
#include "vars.h"
#include "nvs_store.h"

// Puffer müssen "static" sein: die EEZ-Getter geben nur Zeiger darauf zurück.
// 4096  Byte reichen für bis zu 255 Einträge ("255\n" = max. 4 Zeichen).
static char opt_main[4096];
static char opt_task[4096];
static char opt_sub[4096];

/**
 * Schreibt "<prefix> 1\n<prefix> 2\n...<prefix> count" (ohne abschließendes \n) in out.
 * LVGL erwartet die Dropdown-Optionen als einen String, getrennt durch '\n'.
 * Die Auswahl im Dropdown ist weiterhin der Index (0, 1, 2, ...), der Text ist nur Beschriftung.
 */
static void build_options(char *out, size_t size, const char *prefix, uint8_t count) {
    size_t len = 0;
    out[0] = '\0';
    for (uint8_t i = 1; i <= count && len < size; i++) {
        len += snprintf(out + len, size - len, (i < count) ? "%s %u\n" : "%s %u", prefix, i);
    }
}

void ui_apply_config(const GroupConfig &c) {
    build_options(opt_main, sizeof(opt_main), "Hauptgruppe", c.mainGroups);
    build_options(opt_task, sizeof(opt_task), "Aufgabe",     c.tasks);
    build_options(opt_sub,  sizeof(opt_sub),  "Untergruppe", c.subGroups);

    set_var_main_group_text(opt_main);
    set_var_question_text(opt_task);     // Aufgaben
    set_var_sub_group_text(opt_sub);     // Untergruppen

    // Auswahl zurücksetzen, damit kein Index übrig bleibt, den es nach dem Neuladen nicht mehr gibt
    set_var_active_main_group(0);
    set_var_active_question(0);
    set_var_active_sub_group(0);
}

void ui_config_init_from_nvs() {
    GroupConfig c;
    if (nvs_load_config(c)) ui_apply_config(c);
}