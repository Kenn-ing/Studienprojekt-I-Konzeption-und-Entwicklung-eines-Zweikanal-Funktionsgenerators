#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_ACTIVE_GROUP = 0,
    FLOW_GLOBAL_VARIABLE_SD_CARD_INSERTED = 1,
    FLOW_GLOBAL_VARIABLE_SD_CARD_SIZE = 2
};

// Native global variables

extern int32_t get_var_active_group();
extern void set_var_active_group(int32_t value);
extern bool get_var_sd_card_inserted();
extern void set_var_sd_card_inserted(bool value);
extern const char *get_var_sd_card_size();
extern void set_var_sd_card_size(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/