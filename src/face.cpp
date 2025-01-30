#include <face.h>
#include <lvgl.h>

void Face::make_home_screen(lv_obj_t *home_screen)
{
  home_screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(home_screen, lv_color_make(0, 0, 0), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(home_screen, LV_OPA_COVER, LV_PART_MAIN);
}

void Face::make_arc(lv_obj_t *home_screen, lv_obj_t  *arc)
{
  lv_color_t stored_color = lv_color_make(140, 0, 255);

  // Create the arc
  arc = lv_arc_create(home_screen);
  lv_obj_set_size(arc, 200, 200);
  lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc, 20, LV_PART_MAIN);
  lv_arc_set_bg_angles(arc, 120, 60);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_set_style_arc_color(arc, lv_color_make(60, 60, 60), LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc, lv_color_make(60, 60, 60), LV_PART_INDICATOR);
  lv_arc_set_range(arc, 0, 100);
  lv_arc_set_value(arc, 100);
  lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
}