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
    FLOW_GLOBAL_VARIABLE_MAIN_GROUP_TEXT = 0,
    FLOW_GLOBAL_VARIABLE_SD_CARD_INSERTED = 1,
    FLOW_GLOBAL_VARIABLE_SD_CARD_SIZE = 2,
    FLOW_GLOBAL_VARIABLE_QUESTION_TEXT = 3,
    FLOW_GLOBAL_VARIABLE_SUB_GROUP_TEXT = 4,
    FLOW_GLOBAL_VARIABLE_ACTIVE_MAIN_GROUP = 5,
    FLOW_GLOBAL_VARIABLE_ACTIVE_QUESTION = 6,
    FLOW_GLOBAL_VARIABLE_ACTIVE_SUB_GROUP = 7,
    FLOW_GLOBAL_VARIABLE_VERSION_NUMBER_TEXT = 8
};

// Native global variables

extern const char *get_var_main_group_text();
extern void set_var_main_group_text(const char *value);
extern bool get_var_sd_card_inserted();
extern void set_var_sd_card_inserted(bool value);
extern const char *get_var_sd_card_size();
extern void set_var_sd_card_size(const char *value);
extern const char *get_var_question_text();
extern void set_var_question_text(const char *value);
extern const char *get_var_sub_group_text();
extern void set_var_sub_group_text(const char *value);
extern int32_t get_var_active_main_group();
extern void set_var_active_main_group(int32_t value);
extern int32_t get_var_active_question();
extern void set_var_active_question(int32_t value);
extern int32_t get_var_active_sub_group();
extern void set_var_active_sub_group(int32_t value);
extern const char *get_var_version_number_text();
extern void set_var_version_number_text(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/