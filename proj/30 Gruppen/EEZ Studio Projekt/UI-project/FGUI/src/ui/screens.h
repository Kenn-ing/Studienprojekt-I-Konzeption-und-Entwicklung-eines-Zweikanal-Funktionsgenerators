#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_STARTING = 1,
    SCREEN_ID_MAIN = 2,
    SCREEN_ID_HELP_OVERVIEW = 3,
    SCREEN_ID_HELP_AMPLITUDE_MESSEN = 4,
    SCREEN_ID_HELP_FREQUENZ_MESSEN = 5,
    SCREEN_ID_HELP_PHASE_MESSEN = 6,
    SCREEN_ID_HELP_GRENZWERTE = 7,
    SCREEN_ID_HELP_INFOS_ZUR_SD_KARTE = 8,
    SCREEN_ID_HELP_INFO = 9,
    _SCREEN_ID_LAST = 9
};

typedef struct _objects_t {
    lv_obj_t *starting;
    lv_obj_t *main;
    lv_obj_t *help_overview;
    lv_obj_t *help_amplitude_messen;
    lv_obj_t *help_frequenz_messen;
    lv_obj_t *help_phase_messen;
    lv_obj_t *help_grenzwerte;
    lv_obj_t *help_infos_zur_sd_karte;
    lv_obj_t *help_info;
    lv_obj_t *starting_text;
    lv_obj_t *spinner;
    lv_obj_t *background;
    lv_obj_t *header;
    lv_obj_t *obj0;
    lv_obj_t *load_data_button;
    lv_obj_t *help_button;
    lv_obj_t *gruppenauswahl;
    lv_obj_t *stop_button;
    lv_obj_t *lable_stop;
    lv_obj_t *start_button;
    lv_obj_t *lable_start;
    lv_obj_t *output_status;
    lv_obj_t *status_led;
    lv_obj_t *obj1;
    lv_obj_t *background_help_overview;
    lv_obj_t *header_help_overview;
    lv_obj_t *exit_button_help_overview;
    lv_obj_t *obj2;
    lv_obj_t *button_aplitude_messen_help_overview;
    lv_obj_t *button_frequenz_messen_help_overview;
    lv_obj_t *button_phase_messen_help_overview;
    lv_obj_t *button_grenzwerte_help_overview;
    lv_obj_t *button_infos_zur_sd_karte_help_overview;
    lv_obj_t *button6_info;
    lv_obj_t *obj3;
    lv_obj_t *background_help_amplitude_messen;
    lv_obj_t *header_help_amplitude_messen;
    lv_obj_t *titel_amplitude_messen;
    lv_obj_t *exit_button_help_overview_1;
    lv_obj_t *background_help_amplitude_messen_2;
    lv_obj_t *header_help_amplitude_messen_2;
    lv_obj_t *exit_button_help_overview_3;
    lv_obj_t *titel_frequenz_messen;
    lv_obj_t *background_help_amplitude_messen_1;
    lv_obj_t *header_help_amplitude_messen_1;
    lv_obj_t *exit_button_help_overview_2;
    lv_obj_t *titel_phase_messen;
    lv_obj_t *background_help_amplitude_messen_3;
    lv_obj_t *header_help_amplitude_messen_3;
    lv_obj_t *exit_button_help_overview_4;
    lv_obj_t *titel_grenzwerte;
    lv_obj_t *background_help_amplitude_messen_4;
    lv_obj_t *header_help_amplitude_messen_4;
    lv_obj_t *exit_button_help_overview_5;
    lv_obj_t *titel_infos_zur_sd_karte;
    lv_obj_t *background_help_info;
    lv_obj_t *header_help_info;
    lv_obj_t *titel_info;
    lv_obj_t *exit_button_help_overview_6;
} objects_t;

extern objects_t objects;

void create_screen_starting();
void tick_screen_starting();

void create_screen_main();
void tick_screen_main();

void create_screen_help_overview();
void tick_screen_help_overview();

void create_screen_help_amplitude_messen();
void tick_screen_help_amplitude_messen();

void create_screen_help_frequenz_messen();
void tick_screen_help_frequenz_messen();

void create_screen_help_phase_messen();
void tick_screen_help_phase_messen();

void create_screen_help_grenzwerte();
void tick_screen_help_grenzwerte();

void create_screen_help_infos_zur_sd_karte();
void tick_screen_help_infos_zur_sd_karte();

void create_screen_help_info();
void tick_screen_help_info();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/