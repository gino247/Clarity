#define LGFX_USE_V1

#include "main.h"

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9A01 _panel_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Bus_SPI _bus_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read = 20000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = SCLK;
      cfg.pin_mosi = MOSI;
      cfg.pin_miso = MISO;
      cfg.pin_dc = DC;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs = CS;
      cfg.pin_rst = RST;
      cfg.pin_busy = -1;

      cfg.memory_width = WIDTH;
      cfg.memory_height = HEIGHT;
      cfg.panel_width = WIDTH;
      cfg.panel_height = HEIGHT;
      cfg.offset_x = OFFSET_X;
      cfg.offset_y = OFFSET_Y;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = true;
      cfg.rgb_order = RGB_ORDER;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = BL;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 1;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
};

LGFX tft;
Preferences prefs;

// Screens
// Screens
lv_obj_t *startup_scr;              // Black startup screen
lv_obj_t *splash_scr;               // Splash screen
lv_obj_t *splash_scr_b;             // Splash screen
lv_obj_t *home_screen;

// Global styles
static lv_style_t style_unit_text;
static lv_style_t style_icon;

//components
Meter *meter;

//screen stuff
static const uint32_t screenWidth = WIDTH;
static const uint32_t screenHeight = HEIGHT;

//constants
lv_color_t black = lv_color_black();
lv_color_t white = lv_color_white();

const unsigned int lvBufferSize = screenWidth * 10;
uint8_t lvBuffer[2][lvBufferSize];

// Screen Dimming
const uint8_t DIM_BRIGHTNESS = 10;
const uint8_t DEFAULT_BRIGHTNESS = 100;
bool isScreenDimmed = false;

// LVGL Time
hw_timer_t* timer = nullptr;

void fade_in_home(lv_timer_t *timer) {
  lv_scr_load_anim(home_screen, LV_SCR_LOAD_ANIM_FADE_IN, 1500, 50, false);
}

void fade_out_splash(lv_timer_t *timer) {
  lv_scr_load_anim(startup_scr, LV_SCR_LOAD_ANIM_FADE_IN, 1000, 0, false);

  lv_timer_t *exit_timer = lv_timer_create(fade_in_home, 2000, home_screen); // back to blank
  lv_timer_set_repeat_count(exit_timer, 1);

}


void set_needle_line_value(void * obj, int32_t v)
{
    meter->set_needle_line_value(obj, v);
}

void screenBrightness(uint8_t value)
{
  tft.setBrightness(value);
}
void checkScreenDimming()
{
  // TODO: wire up the 'false' to a check if screen should be dimmed
  if (!isScreenDimmed && false)
  {
    // Time to dim the screen
    screenBrightness(DIM_BRIGHTNESS);
    isScreenDimmed = true;
  }
}
void resetScreenBrightness()
{
  if (isScreenDimmed)
  {
    screenBrightness(DEFAULT_BRIGHTNESS);
    isScreenDimmed = false;
  }
}

// TODO: is this required?
void my_disp_flush(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  lv_draw_sw_rgb565_swap(data, w * h);

  if (tft.getStartCount() == 0)
  {
    tft.endWrite();
  }

  tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
  lv_disp_flush_ready(display);
}

// Objects
lv_obj_t *arc;

// Elements for Home Screen
void make_arc()
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

void make_startup_screen() 
{
  startup_scr = lv_screen_active(); // Make startup screen active
  lv_obj_set_style_bg_color(startup_scr, black, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(startup_scr, LV_OPA_COVER, LV_PART_MAIN);
}

// Make Home Screen
void make_home_screen()
{
  home_screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(home_screen, black, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(home_screen, LV_OPA_COVER, LV_PART_MAIN);
}

void init_screen(void)
{
    // initialise screen
    tft.init();
    tft.initDMA();
    tft.startWrite();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(prefs.getInt("rotate", 0));

    lv_init(); // initialise LVGL
    
    // setup screen
    static auto *lvDisplay = lv_display_create(screenWidth, screenHeight);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, my_disp_flush);
    lv_display_set_buffers(lvDisplay, lvBuffer[0], lvBuffer[1], lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    screenBrightness(prefs.getInt("brightness", 100)); // startup brightness

}

// Styles
void make_styles(void) {
  lv_style_init(&style_unit_text);
  lv_style_set_text_font(&style_unit_text, &ubuntu_24);
  lv_style_set_text_color(&style_unit_text, white);

  lv_style_init(&style_icon);
  lv_style_set_text_font(&style_icon, &font_awesome_icons_small);
  lv_style_set_text_color(&style_icon, lv_palette_main(LV_PALETTE_GREY));
}

void make_splash_screen(void) {
  splash_scr = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(splash_scr, black, 0);

  lv_obj_t *icon_three = lv_img_create(splash_scr);
  lv_img_set_src(icon_three, LV_SYMBOL_DUMMY "Clarity"); //&image1_0_resized);
  lv_obj_set_style_text_color(icon_three, white, LV_OPA_0);
  lv_obj_align(icon_three, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_img_recolor(icon_three, white, 0);
  lv_obj_set_style_img_recolor_opa(icon_three, LV_OPA_COVER, 0);
}

// void IRAM_ATTR onTimer() {
//   lv_tick_inc(1);
// }

void timer_init() {
  // const uint8_t lv_tick_frequency = 1000;  // 1 kHz = 1ms period

  // timer = timerBegin(lv_tick_frequency, 1, true);  // Configure the timer with 1kHz frequency
  // if (!timer) {
  //   Serial.println("Failed to configure timer!");
  //   while (1)
  //     ;  // Halt execution on failure
  // }

  // timerAttachInterrupt(timer, &onTimer);  // Attach the ISR to the timer
  // Serial.println("Timer initialized for LVGL tick");
}

void setup()
{
  try
  {
    Serial.begin(115200);

    init_screen();
    make_startup_screen();

    make_styles();

    make_splash_screen();

    make_home_screen();
    
    //init Meter
    meter = new Meter(home_screen);
    meter->register_animation_cb(set_needle_line_value);
    meter->build();

    // lv_scr_load(splash_scr);
    lv_scr_load_anim(splash_scr, LV_SCR_LOAD_ANIM_FADE_IN, 1500, 50, false);

    lv_timer_t *exit_timer = lv_timer_create(fade_out_splash, 2000, home_screen); // back to blank
    lv_timer_set_repeat_count(exit_timer, 1);

    timer_init();

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

    // checkScreenDimming();
    // delay(5);
  }
  catch (const std::exception &e)
  {
    Serial.println(e.what());
    throw;
  }
}