#ifndef MAIN_H
#define MAIN_H

#define UI_VERSION "4.1.0"

#include "Arduino.h"
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "fonts/ubuntu_24.h"
#include "fonts/ubuntu_100.h"
#include "fonts/ubuntu_200.h"
#include "fonts/font_awesome_icons_small.h"
#include <string.h>
#include <Preferences.h>

#include "images/splash_2.h"

#include "meter.h"

// screen configs
#define WIDTH 240
#define HEIGHT 240
#define OFFSET_X 0
#define OFFSET_Y 0
#define RGB_ORDER false

// display
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

#endif // MAIN_H