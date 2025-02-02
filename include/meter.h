#ifndef METER_H
#define METER_H

#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <misc/lv_event.h>

#ifdef __cplusplus
extern "C" {
#endif

class Meter
{
  public:
    Meter(lv_obj_t *screen);
    void register_animation_cb(lv_anim_exec_xcb_t cb);
    void build(void);
    void set_needle_line_value(void * obj, int32_t v);
  private:
    lv_obj_t *display;
    lv_obj_t *needle_line;
    lv_anim_exec_xcb_t animation_cb;
};

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // METER_H