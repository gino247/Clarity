[![Build ESP32 Project with PlatformIO](https://github.com/marcelrienks/Clarity/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/marcelrienks/Clarity/actions/workflows/c-cpp.yml)

# Clarity
An ESP32 project, using platformio, which builds a custom digital gauge for monitoring and displaying your engines key parameters, on various screen configurations and combinations of components and sensors.

## Main Libraries:
* Arduino
* [LVGL](https://docs.lvgl.io/master/)
* [LovyanGFX](https://docs.arduino.cc/libraries/lovyangfx/)

## Architecture:
Conceptually there is one `device`, with one `display` (this is based on lvgl structure), this can have many `screen` configurations, each of which having many `components` together comprising a screen. _Note: components can be part of many screen configurations (many to many relationship)_
> Device > Display > Screens > Components

This is designed around an MVP pattern.  
A sensor will represent the model, and be responsible for handling data reading. A component will represent the view, and be responsible for building the view for the sensor data. And the screen will represent the presenter, and will be responsible for building and displaying one or more components per screen.
> Sensor(model) <> Component(view) <> Screen(presenter)  

Based on an input (either from sensor signal or from a button press) the device/display will load different screens. For example an emergency screen can be shown when a sensor signal moves beyond a threshold, replacing the screen that was configured to display using a button input which normally cycles through multiple screen configurations.

_This would also allow for future extension with more screen configurations and sensors._

## Screens list:
### Current:
* _WIP_
### Future (TODO):
* **Oil pressure gauge**  
Displaying an analogue oil pressure gauge, key check symbol, and immobiliser symbol. Fully replacing the stock gauge design of the car (specific to Mazda mx5 NC)
* **Oil pressure warning**

## Components list:
### Current:
* _WIP_
### Future (TODO):
* **Oil pressure gauge**
* **Oil pressure warning**
* **Key check**
* **Immobiliser check**

## Component Parts:
These are the components used for this specific project, but the code can be adjusted to suit any combination of microcontroller and lcd display.
* NodeMCU-32
* 1.28" round display (GC9A01 driver)

## Partitions:
This is specific to an NodeMCU-32S dev board, using a ESP32-WROOM-32 (ESP32-D0WD-V3 chip) with 32Mb(4MB) flash
> - **nvs:** Minimal non-volatile storage for key-value pairs (12 KB).
> - **otadata:** OTA data partition (8 KB).
> - **ota_0:** Application partition for OTA updates (2 MB).
> - **coredump:** Minimal core dump partition (64 KB).
> - **fat:** FAT filesystem partition using the remaining space (1.875 MB).

## Credits:
This project took inspiration from a product that [Rotarytronics](https://www.rotarytronics.com/) has already built and can be purchased online.

The code is also based on other projects from:
* [Garage Tinkering](https://github.com/valentineautos)
* [fbiego](https://github.com/fbiego)

As well as contributions from [Eugene Petersen](https://github.com/gino247)
