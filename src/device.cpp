#include <device.h>
#include <LovyanGFX.hpp>

Device::Device()
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

void Device::screen_brightness(uint8_t value)
{
    setBrightness(value);
}

void Device::reset_screen_brightness()
{
  if (is_screen_dimmed)
  {
    screen_brightness(DEFAULT_BRIGHTNESS);
    is_screen_dimmed = false;
  }
}

void Device::check_screen_dimming()
{
  // TODO: wire up the 'false' to a check if screen should be dimmed
  if (!is_screen_dimmed && false)
  {
    // Time to dim the screen
    screen_brightness(DIM_BRIGHTNESS);
    is_screen_dimmed = true;
  }
}
