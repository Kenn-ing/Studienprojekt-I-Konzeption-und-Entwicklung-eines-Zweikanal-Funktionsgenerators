/**
 * @file actions.cpp
 * @brief Implementierung der Aktionen für die Tasten auf dem Touchscreen.
 * Die Aktionen umfassen das Starten und Stoppen der Ausgabe, das Laden von Daten, das Anzeigen von Hilfeseiten und das Wechseln zwischen Bildschirmen. 
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. 
 * Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
//----------------------------------------------------------------------

#include "actions.h"
#include "screens.h"
#include <lvgl.h>
#include "vars.h"
#include "ui.h"
#include "buzzer.h"
#include "csv.h"
#include "signal_config.h"
#include "Uart.h"
#include <esp_timer.h>
extern volatile int64_t time_touch;  // Variable für die Messung der Reaktionsgeschwindigkeit


//----------------------------------------------------------------------

static uint32_t last_press_time = 0;  // Timer für die Entprellung der Tasten
const uint32_t DEBOUNCE_MS = 100;     // Entprellungszeit in Millisekunden
static SignalGroup groups[MAX_GROUPS];
static uint8_t group_count = 0;

//----------------------------------------------------------------------

/**
 * Fehler-Popups erstellen.
 * Wenn die CSV-Datei fehlerhaft oder nicht vorhanden ist, wird ein rotes Popup mit einer Fehlermeldung angezeigt.
 * Wenn ein Fehler in der UART-Kommunikation auftritt, wird ebenfalls ein rotes Popup mit einer entsprechenden Fehlermeldung angezeigt.
 * Beide Popups können durch Antippen geschlossen werden.
 */
static void create_error_data(void) {
    // Altes Popup löschen falls noch vorhanden
    if (objects.error_data) {
        lv_obj_del(objects.error_data);
        objects.error_data = NULL;
    }

    lv_obj_t *obj = lv_msgbox_create(lv_scr_act(), "", "", 0, true);
    objects.error_data = obj;
    lv_obj_set_pos(obj, 31, 38);
    lv_obj_set_size(obj, 180, 142);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *label = lv_label_create(obj);
        lv_obj_set_pos(label, 0, 0);
        lv_obj_set_size(label, 151, LV_SIZE_CONTENT);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(label, "ERROR!\nCSV-Datei fehlerhaft oder nicht vorhanden.");
    }
    lv_obj_add_event_cb(obj, [](lv_event_t* e) {
    objects.error_data = NULL;
    }, LV_EVENT_DELETE, NULL);
}

static void create_error_uart(void) {
    if (objects.error_uart) {
        lv_obj_del(objects.error_uart);
        objects.error_uart = NULL;
    }

    lv_obj_t *obj = lv_msgbox_create(lv_scr_act(), "", "", 0, true);
    objects.error_uart = obj;
    lv_obj_set_pos(obj, 31, 38);
    lv_obj_set_size(obj, 180, 142);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *label = lv_label_create(obj);
        lv_obj_set_pos(label, 0, 0);
        lv_obj_set_size(label, 151, LV_SIZE_CONTENT);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(label, "ERROR!\nFehler in der UART-Kommunikation");
    }
    lv_obj_add_event_cb(obj, [](lv_event_t* e) {
    objects.error_uart = NULL;
    }, LV_EVENT_DELETE, NULL);
}
//----------------------------------------------------------------------

/**
 * Wenn die Start-Taste gedrückt wird, soll die Status-LED auf volle Helligkeit eingestellt und ein Ton ausgegeben werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_start(lv_event_t* e) {
    int64_t t_action = esp_timer_get_time();
    uint32_t now = millis();
    if (now - last_press_time < DEBOUNCE_MS) return;
    last_press_time = now;
    buzzer_sound();

    uint8_t group = (uint8_t)get_var_active_group() + 1;  // weil EEZ ab 0 zählt jede Gruppe +1
    Serial.printf("Starte Gruppe %u\n", group);
    lv_led_set_brightness(objects.status_led, 255);

    if (!uart_group_start(group)) {
        Serial.println("START-Befehl fehlgeschlagen");
        lv_led_set_brightness(objects.status_led, 0);
        create_error_uart();
    }
}

/**
 * Wenn die Stop-Taste gedrückt wird, soll die Status-LED ausgeschaltet und ein Ton ausgegeben werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_stop(lv_event_t* e) {
    uint32_t now = millis();
    if (now - last_press_time < DEBOUNCE_MS) return;
    last_press_time = now;
    buzzer_sound();

    lv_led_set_brightness(objects.status_led, 0);
    uart_group_stop();
}

/**
 * Wenn die Load Data-Taste gedrückt wird, soll ein Ton ausgegeben werden und die Daten geladen werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_load_data(lv_event_t* e) {
    uint32_t now = millis();
    if (now - last_press_time < DEBOUNCE_MS) return;
    last_press_time = now;
    buzzer_sound();

    if (!csv_read(groups, group_count)) {
        Serial.println("Fehler beim Laden der CSV");
        create_error_data();
        return;
    }

    Serial.println("CSV geladen, sende per UART...");
    for (uint8_t i = 0; i < group_count; i++) {
        csv_debug(groups[i]);
    }

    if (!uart_send_groups(groups, group_count)) {
        Serial.println("UART-Übertragung fehlgeschlagen");
        create_error_uart();
        return;
    }

    Serial.println("Alle Gruppen erfolgreich übertragen");

}

/**
 * Wenn die Help-Taste gedrückt wird, soll ein Ton ausgegeben werden und die Hilfeseite geladen werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_help(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_OVERVIEW);
}

/**
 * Wenn die Exit-to-Main-Taste gedrückt wird, soll ein Ton ausgegeben werden und zum Hauptbildschirm gewechselt werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_exit_to_main(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und zum Hauptbildschirm wechseln
  buzzer_sound();
  loadScreen(SCREEN_ID_MAIN);
}

/**
 * Wenn die Help-Taste auf einer Hilfeseite gedrückt wird, soll ein Ton ausgegeben werden und die entsprechende Hilfeseite geladen werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_show_help_aplitude_messen(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_AMPLITUDE_MESSEN);
}

extern "C" void action_show_help_frequenz_messen(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_FREQUENZ_MESSEN);
}

extern "C" void action_show_help_phase_messen(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_PHASE_MESSEN);
}

extern "C" void action_show_help_grenzwerte(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_GRENZWERTE);
}

extern "C" void action_show_help_infos_zur_sd_karte(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_INFOS_ZUR_SD_KARTE);
}

extern "C" void action_show_help_info(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und entsprechende Hilfeseite laden
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_INFO);
}

/**
 * Wenn die Exit-to-Help-Overview-Taste auf einer Hilfeseite gedrückt wird, soll ein Ton ausgegeben werden und zur Hilfsübersicht gewechselt werden.
 * Für die Entprellung der Tasten wird ein Timer verwendet, der die Zeit seit der letzten Betätigung der Taste misst. Wenn die Taste innerhalb der Entprellungszeit erneut gedrückt wird, wird die Aktion ignoriert.
 */
extern "C" void action_exit_to_help_overview(lv_event_t* e) {
  // Überprüfen, ob die Taste innerhalb der Entprellungszeit erneut gedrückt wurde
  uint32_t now = millis();
  if (now - last_press_time < DEBOUNCE_MS) return;
  last_press_time = now;

  // Aktion ausführen: Ton ausgeben und zum Hauptbildschirm wechseln
  buzzer_sound();
  loadScreen(SCREEN_ID_HELP_OVERVIEW);
}
