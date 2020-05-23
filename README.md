# LittlevGL ESP32 Arduino library

LittlevGL ported to the ESP32 as an Arduino library.

Supported display controllers:

- ILI9341
- ILI9488 (not tested)
- ILI9486 (not tested)
- HX8357B/HX8357D (not tested)
- ST7789 (not tested)
- SH1107 (not tested)
- SSD1306 (not tested)

Supported touchscreen controllers:

- XPT2046 (not tested)
- FT3236 (not tested)
- other FT6X36 or the FT6206 controllers should work as well (not tested)
- STMPE610 (not tested)

## Get started

Running on 4.0 Dev Branch.

Configuration files
lvgl_hw_config.h to configure display and touch type.
lv_examples/lv_ex_conf_templ.h to configure examples. 
 
Example result should look like this:

![LVGL Arduino Widgets Demo](extras/widgets_demo.mp4)
 