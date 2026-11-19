#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_stop(lv_event_t * e);
extern void action_start(lv_event_t * e);
extern void action_load_data(lv_event_t * e);
extern void action_help(lv_event_t * e);
extern void action_exit_to_main(lv_event_t * e);
extern void action_show_help_aplitude_messen(lv_event_t * e);
extern void action_show_help_frequenz_messen(lv_event_t * e);
extern void action_show_help_phase_messen(lv_event_t * e);
extern void action_show_help_grenzwerte(lv_event_t * e);
extern void action_show_help_infos_zur_sd_karte(lv_event_t * e);
extern void action_exit_to_help_overview(lv_event_t * e);
extern void action_show_help_info(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/