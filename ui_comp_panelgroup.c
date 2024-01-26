// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project_3

#include "ui.h"


// COMPONENT Panelgroup

lv_obj_t * ui_Panelgroup_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Panelgroup;
    cui_Panelgroup = lv_obj_create(comp_parent);
    lv_obj_set_width(cui_Panelgroup, 622);
    lv_obj_set_height(cui_Panelgroup, 100);
    lv_obj_set_align(cui_Panelgroup, LV_ALIGN_CENTER);
    lv_obj_clear_flag(cui_Panelgroup, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(cui_Panelgroup, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Panelgroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(cui_Panelgroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_Panelgroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_updateTimeSwitch;
    cui_updateTimeSwitch = lv_switch_create(cui_Panelgroup);
    lv_obj_set_width(cui_updateTimeSwitch, 100);
    lv_obj_set_height(cui_updateTimeSwitch, 50);
    lv_obj_set_x(cui_updateTimeSwitch, -232);
    lv_obj_set_y(cui_updateTimeSwitch, -2);
    lv_obj_set_align(cui_updateTimeSwitch, LV_ALIGN_CENTER);

    lv_obj_t * cui_updateTimeLabel;
    cui_updateTimeLabel = lv_label_create(cui_Panelgroup);
    lv_obj_set_width(cui_updateTimeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_updateTimeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cui_updateTimeLabel, 66);
    lv_obj_set_y(cui_updateTimeLabel, -4);
    lv_obj_set_align(cui_updateTimeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(cui_updateTimeLabel, "Update Time Using NTP");
    lv_obj_set_style_text_font(cui_updateTimeLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_PANELGROUP_NUM);
    children[UI_COMP_PANELGROUP_PANELGROUP] = cui_Panelgroup;
    children[UI_COMP_PANELGROUP_UPDATETIMESWITCH] = cui_updateTimeSwitch;
    children[UI_COMP_PANELGROUP_UPDATETIMELABEL] = cui_updateTimeLabel;
    lv_obj_add_event_cb(cui_Panelgroup, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Panelgroup, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_Panelgroup_create_hook(cui_Panelgroup);
    return cui_Panelgroup;
}

