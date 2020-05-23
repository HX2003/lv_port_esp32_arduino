/**
 * @file lvgl_hw_config.h
 *
 */

#ifndef LV_HW_CONFIG_H
#define LV_HW_CONFIG_H

#define CONFIG_LVGL_PREDEFINED_DISPLAY_NONE 1

#define CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
#define CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI
#define CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI 1
#define CONFIG_LVGL_DISP_SPI_MOSI 13
#define CONFIG_LVGL_DISP_SPI_CLK 14
#define CONFIG_LVGL_DISP_SPI_CS 15
#define CONFIG_LVGL_DISP_PIN_DC 2
#define CONFIG_LVGL_DISP_PIN_RST 4
#define CONFIG_LVGL_DISP_PIN_BCKL 27

#define CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE  
//#define CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE_INVERTED  
//#define CONFIG_LVGL_DISPLAY_ORIENTATION_PORTRAIT 
//#define CONFIG_LVGL_DISPLAY_ORIENTATION_PORTRAIT_INVERTED   
#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 240
#define CONFIG_LVGL_DISPLAY_ORIENTATION 2 //TODO

#define CONFIG_LVGL_TOUCH_X_MIN 0 //TODO
#define CONFIG_LVGL_TOUCH_Y_MIN 0 //TODO
#define CONFIG_LVGL_TOUCH_X_MAX 0 //TODO
#define CONFIG_LVGL_TOUCH_Y_MAX 0 //TODO

#define CONFIG_LVGL_TOUCH_I2C_SDA 21 //Not used
#define CONFIG_LVGL_TOUCH_I2C_SCL 22 //Not used
#define CONFIG_LVGL_TOUCH_SPI_CS 5 //Not used
#define CONFIG_LVGL_TOUCH_PIN_IRQ 25 //Not used
/*--END OF LV_HW_CONFIG_H--*/

#endif /*LV_HW_CONFIG_H*/