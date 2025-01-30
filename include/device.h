#include <LovyanGFX.hpp>

// configs
#define WIDTH 240
#define HEIGHT 240
#define OFFSET_X 0
#define OFFSET_Y 0
#define RGB_ORDER false

// nodemcu-32 <> 1.28" round display (GC9A01 driver)
#define SPI SPI2_HOST
#define SCLK 18
#define MOSI 23
#define MISO -1
#define DC 16
#define CS 22
#define RST 4
#define BL 3
#define BUZZER -1
#define MAX_FILE_OPEN 10

class Device : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_GC9A01 _panel_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Bus_SPI _bus_instance;

public:
    const uint8_t DIM_BRIGHTNESS = 10;
    const uint8_t DEFAULT_BRIGHTNESS = 100;
    bool is_screen_dimmed = false;

    Device(void);
    void screen_brightness(uint8_t value);
    void reset_screen_brightness();
    void check_screen_dimming();
};