/**
 * @file vars_groups.cpp
 * @brief KOMPLETT NEU (Master): Implementierung der EEZ-Variablen für Hauptgruppe / Aufgabe / Untergruppe.
 *
 * vars.h (von EEZ Studio erzeugt) deklariert diese Funktionen nur. Die Implementierung muss selbst
 * geschrieben werden. Sie fehlte -> Linker-Fehler "undefined reference to get_var_... / set_var_...".
 *
 * Wichtig: vars.h einbinden! Dort stehen die Funktionen in einem extern "C"-Block. Nur dann bekommen die
 * Definitionen in dieser C++-Datei dieselben (nicht-gemangelten) Namen, die screens.c erwartet.
 */
#include "vars.h"
#include <string.h>

// ---- Dropdown-Optionen (Strings "1\n2\n3...") ------------------------------
// 1024 Byte reichen für bis zu 255 Einträge. Die Getter geben Zeiger auf diese Puffer zurück,
// deshalb müssen sie static (dauerhaft) sein.
static char main_group_text[1024] = "";
static char question_text[1024]   = "";
static char sub_group_text[1024]  = "";

// ---- Aktuelle Auswahl der Dropdowns (0-basiert, EEZ zählt ab 0) ------------
static int32_t active_main_group = 0;
static int32_t active_question   = 0;
static int32_t active_sub_group  = 0;

/**
 * Kopiert value sicher in einen Puffer (wird nie überlaufen, immer nullterminiert).
 */
static void copy_text(char *dst, size_t size, const char *value) {
    if (!value) { dst[0] = '\0'; return; }
    strncpy(dst, value, size - 1);
    dst[size - 1] = '\0';
}

// Hauptgruppen: Optionsliste
const char *get_var_main_group_text() { return main_group_text; }
void set_var_main_group_text(const char *value) { copy_text(main_group_text, sizeof(main_group_text), value); }

// Aufgaben: Optionsliste (EEZ-Variable heißt "question")
const char *get_var_question_text() { return question_text; }
void set_var_question_text(const char *value) { copy_text(question_text, sizeof(question_text), value); }

// Untergruppen: Optionsliste
const char *get_var_sub_group_text() { return sub_group_text; }
void set_var_sub_group_text(const char *value) { copy_text(sub_group_text, sizeof(sub_group_text), value); }

// Ausgewählte Hauptgruppe (0-basiert)
int32_t get_var_active_main_group() { return active_main_group; }
void set_var_active_main_group(int32_t value) { active_main_group = value; }

// Ausgewählte Aufgabe (0-basiert)
int32_t get_var_active_question() { return active_question; }
void set_var_active_question(int32_t value) { active_question = value; }

// Ausgewählte Untergruppe (0-basiert)
int32_t get_var_active_sub_group() { return active_sub_group; }
void set_var_active_sub_group(int32_t value) { active_sub_group = value; }
