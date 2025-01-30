#define LGFX_USE_V1
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <string.h>
#include <Preferences.h>
#include <main.h>
#include <device.h>
#include <face.h>

Device device;
Face face;
Preferences prefs;

const unsigned int buffer_size = WIDTH * 10;
uint8_t buffer[2][buffer_size];

void flush_display(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  lv_draw_sw_rgb565_swap(data, w * h);

  if (device.getStartCount() == 0)
  {
    device.endWrite();
  }

  device.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
  lv_disp_flush_ready(display);
}

// Screens
lv_obj_t *home_screen;

// Objects
lv_obj_t *arc;

void setup()
{
  try
  {
    Serial.begin(115200);

    int rt = prefs.getInt("rotate", 0);
    int br = prefs.getInt("brightness", 100);

    // initialise screen
    device.init();
    device.initDMA();
    device.startWrite();
    device.fillScreen(TFT_BLACK);
    device.setRotation(rt);

    lv_init(); // initialise LVGL

    // setup screen
    static auto *lvDisplay = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, flush_display);
    lv_display_set_buffers(lvDisplay, buffer[0], buffer[1], buffer_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

    device.screen_brightness(br); // startup brightness

    face.make_home_screen(home_screen);
    face.make_arc(home_screen, arc);

    lv_scr_load(home_screen);

    Serial.print("Setup finished");
  }
  catch (const std::exception &e)
  {
    Serial.println(e.what());
    throw;
  }
}

void loop()
{
  try
  {
    static uint32_t lastTick = millis();
    uint32_t current = millis();
    lv_tick_inc(current - lastTick);
    lastTick = current;
    lv_timer_handler();

    device.check_screen_dimming();
    delay(5);
  }
  catch (const std::exception &e)
  {
    Serial.println(e.what());
    throw;
  }
}