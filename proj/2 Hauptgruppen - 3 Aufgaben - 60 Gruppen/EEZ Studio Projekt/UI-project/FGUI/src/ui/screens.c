#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static void event_handler_cb_main_droppdown_main_group(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_dropdown_get_selected(ta);
            set_var_active_main_group(value);
        }
    }
}

static void event_handler_cb_main_droppdown_question(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_dropdown_get_selected(ta);
            set_var_active_question(value);
        }
    }
}

static void event_handler_cb_main_droppdown_sub_group(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_dropdown_get_selected(ta);
            set_var_active_sub_group(value);
        }
    }
}

void create_screen_starting() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.starting = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_1 = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Starting_text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.starting_text = obj;
            lv_obj_set_pos(obj, 43, 139);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Loading...");
        }
        {
            // Spinner
            lv_obj_t *obj = lv_spinner_create(parent_obj, 1000, 60);
            objects.spinner = obj;
            lv_obj_set_pos(obj, 80, 192);
            lv_obj_set_size(obj, 80, 80);
            lv_obj_set_style_arc_width(obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xffd40c2e), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // Version_Text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.version_text = obj;
            lv_obj_set_pos(obj, 194, 290);
            lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 70, 22);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_hwr_logo);
        }
    }
    
    tick_screen_starting();
}

void tick_screen_starting() {
    {
        const char *new_val = get_var_version_number_text();
        const char *cur_val = lv_label_get_text(objects.version_text);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.version_text;
            lv_label_set_text(objects.version_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd50c2f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 211, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_sd_card);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 211, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_crossing);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 153, 11);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // Load_Data_Button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.load_data_button = obj;
                    lv_obj_set_pos(obj, 49, 0);
                    lv_obj_set_size(obj, 89, 36);
                    lv_obj_add_event_cb(obj, action_load_data, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Daten Laden");
                        }
                    }
                }
                {
                    // Help_button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.help_button = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 42, 36);
                    lv_obj_add_event_cb(obj, action_help, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Hilfe");
                        }
                    }
                }
            }
        }
        {
            // Stop_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.stop_button = obj;
            lv_obj_set_pos(obj, 21, 260);
            lv_obj_set_size(obj, 90, 50);
            lv_obj_add_event_cb(obj, action_stop, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Lable_stop
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lable_stop = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "STOP");
                }
            }
        }
        {
            // Start_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.start_button = obj;
            lv_obj_set_pos(obj, 130, 260);
            lv_obj_set_size(obj, 90, 50);
            lv_obj_add_event_cb(obj, action_start, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Lable_start
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lable_start = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "START");
                }
            }
        }
        {
            // Output_Status
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.output_status = obj;
            lv_obj_set_pos(obj, 30, 226);
            lv_obj_set_size(obj, 181, 26);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Status_LED
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    objects.status_led = obj;
                    lv_obj_set_pos(obj, 160, 7);
                    lv_obj_set_size(obj, 14, 14);
                    lv_led_set_color(obj, lv_color_hex(0xff1a7f00));
                    lv_led_set_brightness(obj, 0);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Status der Ausgabe:");
                }
            }
        }
        {
            // Droppdown_Main_Group
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.droppdown_main_group = obj;
            lv_obj_set_pos(obj, 40, 62);
            lv_obj_set_size(obj, 160, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_obj_add_event_cb(obj, event_handler_cb_main_droppdown_main_group, LV_EVENT_ALL, 0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd50c2f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Droppdown_Question
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.droppdown_question = obj;
            lv_obj_set_pos(obj, 40, 187);
            lv_obj_set_size(obj, 160, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_obj_add_event_cb(obj, event_handler_cb_main_droppdown_question, LV_EVENT_ALL, 0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd50c2f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Droppdown_Sub_Group
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.droppdown_sub_group = obj;
            lv_obj_set_pos(obj, 40, 124);
            lv_obj_set_size(obj, 160, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_obj_add_event_cb(obj, event_handler_cb_main_droppdown_sub_group, LV_EVENT_ALL, 0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd50c2f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Label_Main_Group
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_main_group = obj;
            lv_obj_set_pos(obj, 70, 42);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Hauptgruppe");
        }
        {
            // Label_Question
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_question = obj;
            lv_obj_set_pos(obj, 72, 104);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Untergruppe");
        }
        {
            // Label_Sub_Group
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_sub_group = obj;
            lv_obj_set_pos(obj, 89, 167);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Aufgabe");
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 6, 44);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_hwr_logo_main);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    {
        bool new_val = get_var_sd_card_inserted();
        bool cur_val = lv_obj_has_flag(objects.obj1, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj1;
            if (new_val) lv_obj_add_flag(objects.obj1, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj1, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_sd_card_size();
        const char *cur_val = lv_label_get_text(objects.obj0);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj0;
            lv_label_set_text(objects.obj0, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_main_group_text();
        const char *cur_val = lv_dropdown_get_options(objects.droppdown_main_group);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.droppdown_main_group;
            lv_dropdown_set_options(objects.droppdown_main_group, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        if (!(lv_obj_get_state(objects.droppdown_main_group) & LV_STATE_EDITED)) {
            int32_t new_val = get_var_active_main_group();
            int32_t cur_val = lv_dropdown_get_selected(objects.droppdown_main_group);
            if (new_val != cur_val) {
                tick_value_change_obj = objects.droppdown_main_group;
                lv_dropdown_set_selected(objects.droppdown_main_group, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
    {
        const char *new_val = get_var_question_text();
        const char *cur_val = lv_dropdown_get_options(objects.droppdown_question);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.droppdown_question;
            lv_dropdown_set_options(objects.droppdown_question, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        if (!(lv_obj_get_state(objects.droppdown_question) & LV_STATE_EDITED)) {
            int32_t new_val = get_var_active_question();
            int32_t cur_val = lv_dropdown_get_selected(objects.droppdown_question);
            if (new_val != cur_val) {
                tick_value_change_obj = objects.droppdown_question;
                lv_dropdown_set_selected(objects.droppdown_question, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
    {
        const char *new_val = get_var_sub_group_text();
        const char *cur_val = lv_dropdown_get_options(objects.droppdown_sub_group);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.droppdown_sub_group;
            lv_dropdown_set_options(objects.droppdown_sub_group, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        if (!(lv_obj_get_state(objects.droppdown_sub_group) & LV_STATE_EDITED)) {
            int32_t new_val = get_var_active_sub_group();
            int32_t cur_val = lv_dropdown_get_selected(objects.droppdown_sub_group);
            if (new_val != cur_val) {
                tick_value_change_obj = objects.droppdown_sub_group;
                lv_dropdown_set_selected(objects.droppdown_sub_group, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
}

void create_screen_help_overview() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_overview = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff185a9d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Overview
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_overview = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Overview
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_overview = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_Button_Help_Overview
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_main, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 38, 47);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Navigation");
        }
        {
            // Button_Aplitude_Messen_Help_Overview
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_aplitude_messen_help_overview = obj;
            lv_obj_set_pos(obj, 16, 110);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_aplitude_messen, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Amplitude \nMessen");
                }
            }
        }
        {
            // Button_Frequenz_Messen_Help_Overview
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_frequenz_messen_help_overview = obj;
            lv_obj_set_pos(obj, 121, 110);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_frequenz_messen, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Frequenz\nMessen");
                }
            }
        }
        {
            // Button_Phase_Messen_Help_Overview
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_phase_messen_help_overview = obj;
            lv_obj_set_pos(obj, 16, 181);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_phase_messen, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Phase\nMessen");
                }
            }
        }
        {
            // Button_Grenzwerte_Help_Overview
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_grenzwerte_help_overview = obj;
            lv_obj_set_pos(obj, 121, 181);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_grenzwerte, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Grenzwerte");
                }
            }
        }
        {
            // Button_Infos_zur_SD_Karte_Help_Overview
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button_infos_zur_sd_karte_help_overview = obj;
            lv_obj_set_pos(obj, 16, 252);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_infos_zur_sd_karte, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Infos zur\nSD-Karte");
                }
            }
        }
        {
            // Button6_Info
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.button6_info = obj;
            lv_obj_set_pos(obj, 121, 252);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_show_help_info, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff33958), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_width(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(obj, -2, LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Info");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 58, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Bitte Auswahl treffen");
        }
    }
    
    tick_screen_help_overview();
}

void tick_screen_help_overview() {
}

void create_screen_help_amplitude_messen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_amplitude_messen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Amplitude_Messen
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_amplitude_messen = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 596);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Amplitude_Messen
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_amplitude_messen = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Titel_Amplitude_Messen
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_amplitude_messen = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Amplitude Messen");
                }
                {
                    // Exit_Button_Help_Overview_1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_1 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 51);
            lv_obj_set_size(obj, 221, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "1. Durch den Knopf mit der Aufschrift \"Meas\" unter dem Punkt \"Measure\" in der Mitte des Bedienelements des Oszilloskops gelangen Sie in die Messansicht.\n\n2. Durch den linken Knopf unterhalb des Displays kann einer der beiden Kanaele ausgewaehlt werden (\"Source\").\n\n3. Durch den Knopf rechts neben der Kanalauswahl (\"Type\") kann die gewuenschte Messung ausgewaehlt werden.\n\n4. Relevante Messungen der Spannung beinhalten: \"Peak-Peak\", \"Maximum\", \"Minimum\", \"Amplitude\", \"Top\", \"Base\" sowie \"AC RMS\".\n\n5. Durch Drehen des dunkelgrau unterlegten Drehreglers auf der linken Seite des Bedienelements kann die Messung ausgewaehlt und durch Druecken des Drehreglers bestaetigt werden.");
        }
    }
    
    tick_screen_help_amplitude_messen();
}

void tick_screen_help_amplitude_messen() {
}

void create_screen_help_frequenz_messen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_frequenz_messen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Amplitude_Messen_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_amplitude_messen_2 = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 544);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Amplitude_Messen_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_amplitude_messen_2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_Button_Help_Overview_3
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_3 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // Titel_Frequenz_Messen
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_frequenz_messen = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Frequenz Messen");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 51);
            lv_obj_set_size(obj, 221, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "1. Durch den Knopf mit der Aufschrift \"Meas\" unter dem Punkt \"Measure\" in der Mitte des Bedienelements des Oszilloskops gelangen Sie in die Messansicht.\n\n2. Durch den linken Knopf unterhalb des Displays kann einer der beiden Kanaele ausgewaehlt werden (\"Source\").\n\n3. Durch den Knopf rechts neben der Kanalauswahl (\"Type\") kann die gewuenschte Messung ausgewaehlt werden.\n\n4. Durch Drehen des dunkelgrau unterlegten Drehreglers auf der linken Seite des Bedienelements kann die Messung ausgewaehlt und durch Druecken des Drehreglers bestaetigt werden.\n\n5. Waehlen Sie unter \"Type\" den Punkt \"Frequency\" aus.");
        }
    }
    
    tick_screen_help_frequenz_messen();
}

void tick_screen_help_frequenz_messen() {
}

void create_screen_help_phase_messen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_phase_messen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Amplitude_Messen_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_amplitude_messen_1 = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 753);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Amplitude_Messen_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_amplitude_messen_1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_Button_Help_Overview_2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_2 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // Titel_Phase_Messen
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_phase_messen = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Phase Messen");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 51);
            lv_obj_set_size(obj, 221, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "1. Durch den Knopf mit der Aufschrift \"Meas\" unter dem Punkt \"Measure\" in der Mitte des Bedienelements des Oszilloskops gelangen Sie in die Messansicht.\n\n2. Durch den linken Knopf (\"Source\") unterhalb des Displays kann der Bezugskanal, von welchem die Messung durchgefuehrt wird, ausgewaehlt werden.\n\n3. Durch den vierten Knopf von links unterhalb des Displays (\"Settings\") kann unter dem Punkt \"Source2\" der zweite Kanal ausgewaehlt werden.\n\n4. Durch das Druecken der Taste \"Back\" unterhalb des Displays gelangen Sie in die vorherige Ansicht.\n\n5. Durch den Knopf rechts neben der Kanalauswahl (\"Type\") kann die gewuenschte Messung ausgewaehlt werden.\n\n6. Durch Drehen des dunkelgrau unterlegten Drehreglers auf der linken Seite des Bedienelements kann die Messung ausgewaehlt und durch Druecken des Drehreglers bestaetigt werden.\n\n7. Waehlen Sie unter \"Type\" den Punkt \"Phase\" aus.");
        }
    }
    
    tick_screen_help_phase_messen();
}

void tick_screen_help_phase_messen() {
}

void create_screen_help_grenzwerte() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_grenzwerte = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Amplitude_Messen_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_amplitude_messen_3 = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Amplitude_Messen_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_amplitude_messen_3 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_Button_Help_Overview_4
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_4 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // Titel_Grenzwerte
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_grenzwerte = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Grenzwerte");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 46);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Freqenzbereich");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 62);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "XX Hz - XX Hz");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 101);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Amplitudenbereich");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "XX - XX V");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 163);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Spannungsabweichung");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 179);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "< +-5 %");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 219);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Frequenzabweichung");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 235);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "< +-5 %");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 280);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Phasenabweichung");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 296);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "< +-2°");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 62);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SIN:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 78);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SQU:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SIN:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 133);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SQU:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 179);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SIN/SQU:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 235);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SIN/SQU:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 296);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SIN/SQU:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 120, 78);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "XX Hz - XX Hz");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 121, 133);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "XX - XX V");
        }
    }
    
    tick_screen_help_grenzwerte();
}

void tick_screen_help_grenzwerte() {
}

void create_screen_help_infos_zur_sd_karte() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_infos_zur_sd_karte = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Amplitude_Messen_4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_amplitude_messen_4 = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Amplitude_Messen_4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_amplitude_messen_4 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_Button_Help_Overview_5
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_5 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // Titel_Infos_zur_SD_Karte
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_infos_zur_sd_karte = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Infos zur SD-Karte");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 42);
            lv_obj_set_size(obj, 221, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Dateiname / Speicherformat: \n\"Gruppen.csv\" / FAT32\n\nTabellenstruktur (->=Zeile):\n-> Gesamtzahl Hauptgruppen, Gesamtzahl Aufgaben, Gesamtzahl Untergruppen\n-> [---, ---, ---]\n-> Hauptgruppe, Aufgabe, Untergruppe, Amplitude CH1, Frequenz CH1, Phase CH1, Signal CH1, Amplitude CH2, Frequenz CH2, Phase CH2, Signal CH2\n-> [---, ---, ---,  ---,  ---,  ---, ---,  ---,  ---,  ---, ---]");
        }
    }
    
    tick_screen_help_infos_zur_sd_karte();
}

void tick_screen_help_infos_zur_sd_karte() {
}

void create_screen_help_info() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.help_info = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Background_Help_Info
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.background_help_info = obj;
            lv_obj_set_pos(obj, 0, -1);
            lv_obj_set_size(obj, 240, 321);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff43040f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 130, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff8c081e), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Header_Help_Info
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header_help_info = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 38);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd40c2e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Titel_Info
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.titel_info = obj;
                    lv_obj_set_pos(obj, 9, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Info");
                }
                {
                    // Exit_Button_Help_Overview_6
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.exit_button_help_overview_6 = obj;
                    lv_obj_set_pos(obj, 171, 0);
                    lv_obj_set_size(obj, 69, 36);
                    lv_obj_add_event_cb(obj, action_exit_to_help_overview, LV_EVENT_RELEASED, (void *)0);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_exit, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd0d0d0), LV_PART_MAIN | LV_STATE_PRESSED);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 51);
            lv_obj_set_size(obj, 221, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Dieser Funktionsgenerator wurde als Teil des Moduls Studienprojekt I (IE-M26-6045) unter der Betreuung von Prof. Dr. Luis F. F. Furtado von den Studierenden Dejan Dimitrijevic und Alexander Klose (Jahrgang 2024) erstellt.");
        }
    }
    
    tick_screen_help_info();
}

void tick_screen_help_info() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_starting,
    tick_screen_main,
    tick_screen_help_overview,
    tick_screen_help_amplitude_messen,
    tick_screen_help_frequenz_messen,
    tick_screen_help_phase_messen,
    tick_screen_help_grenzwerte,
    tick_screen_help_infos_zur_sd_karte,
    tick_screen_help_info,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_starting();
    create_screen_main();
    create_screen_help_overview();
    create_screen_help_amplitude_messen();
    create_screen_help_frequenz_messen();
    create_screen_help_phase_messen();
    create_screen_help_grenzwerte();
    create_screen_help_infos_zur_sd_karte();
    create_screen_help_info();
}
