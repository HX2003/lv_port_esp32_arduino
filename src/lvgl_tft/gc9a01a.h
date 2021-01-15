/**
 * @file lv_templ.h
 *
 */

#ifndef GC9A01A_H
#define GC9A01A_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

#include "lvgl/lvgl.h"
#include "../lvgl_driver.h"

/*********************
 *      DEFINES
 *********************/
#define GC9A01A_DC   CONFIG_LVGL_DISP_PIN_DC
#define GC9A01A_RST  CONFIG_LVGL_DISP_PIN_RST
#define GC9A01A_BCKL CONFIG_LVGL_DISP_PIN_BCKL

#define GC9A01A_ENABLE_BACKLIGHT_CONTROL CONFIG_LVGL_ENABLE_BACKLIGHT_CONTROL

#if CONFIG_LVGL_BACKLIGHT_ACTIVE_LVL
  #define GC9A01A_BCKL_ACTIVE_LVL 1
#else
  #define GC9A01A_BCKL_ACTIVE_LVL 0
#endif

#define GC9A01A_INVERT_COLORS CONFIG_LVGL_INVERT_COLORS

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void gc9a01a_init(void);
void gc9a01a_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void gc9a01a_enable_backlight(bool backlight);
void gc9a01a_sleep_in(void);
void gc9a01a_sleep_out(void);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GC9A01A_H*/
