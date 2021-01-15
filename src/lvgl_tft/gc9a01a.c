/**
 * @file gc9a01a.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "gc9a01a.h"
#include "disp_spi.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*********************
 *      DEFINES
 *********************/
 #define TAG "GC9A01A"

/**********************
 *      TYPEDEFS
 **********************/

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void gc9a01a_set_orientation(uint8_t orientation);

static void gc9a01a_send_cmd(uint8_t cmd);
static void gc9a01a_send_data(void * data, uint16_t length);
static void gc9a01a_send_color(void * data, uint32_t length);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void gc9a01a_init(void)
{
	lcd_init_cmd_t ili_init_cmds[]={
		{0xEF, {0}, 0},
		{0xEB, {0x14}, 1},
		{0xFE, {0}, 0},
		{0xEF, {0}, 0},
		{0x84, {0x40}, 1}, 
		{0x85, {0xFF}, 1}, 
		{0x86, {0xFF}, 1}, 
		{0x87, {0xFF}, 1}, 
		{0x88, {0x0A}, 1}, 
		{0x89, {0x21}, 1}, 
		{0x8A, {0x00}, 1}, 
		{0x8B, {0x80}, 1}, 
		{0x8C, {0x01}, 1}, 
		{0x8D, {0x01}, 1}, 
		{0x8E, {0xFF}, 1}, 
		{0x8F, {0xFF}, 1}, 
		{0xB6, {0x00,0x00}, 2}, 
		{0x36, {0x48}, 1}, //Orientation
		{0x3A, {0x05}, 1},
		{0x90, {0x08,0x08,0x08,0x08}, 4},
		{0xBD, {0x06}, 1},
		{0xBC, {0x00}, 1},
		{0xFF, {0x60, 0x01, 0x04}, 3},
		{0xC3, {0x13}, 1},
		{0xC4, {0x13}, 1},
		{0xC9, {0x22}, 1},
		{0xBE, {0x11}, 1},
		{0xE1, {0x10,0x0E}, 2},
		{0xDF, {0x21,0x0c,0x02}, 3},
		{0xF0, {0x45,0x09,0x08,0x08,0x26,0x2A}, 6},
		{0xF1, {0x43,0x70,0x72,0x36,0x37,0x6F}, 6},
		{0xF2, {0x45,0x09,0x08,0x08,0x26,0x2A}, 6},
		{0xF3, {0x43,0x70,0x72,0x36,0x37,0x6F}, 6}, 
		{0xED, {0x1B,0x0B}, 2},
		{0xAE, {0x77}, 1},
		{0xCD, {0x63}, 1},
		{0x70, {0x07,0x07,0x04,0x0E,0x0F,0x09,0x07,0x08,0x03}, 9},
		{0xE8, {0x34}, 1},
		{0x62, {0x18,0x0D,0x71,0xED,0x70,0x70,0x18,0x0F,0x71,0xEF,0x70,0x70}, 12},
		{0x63, {0x18,0x11,0x71,0xF1,0x70,0x70,0x18,0x13,0x71,0xF3,0x70,0x70}, 12},
		{0x64, {0x28,0x29,0xF1,0x01,0xF1,0x00,0x07}, 7},
		{0x66, {0x3C,0x00,0xCD,0x67,0x45,0x45,0x10,0x00,0x00,0x00}, 10},
		{0x67, {0x00,0x3C,0x00,0x00,0x00,0x01,0x54,0x10,0x32,0x98}, 10},
		{0x74, {0x10,0x85,0x80,0x00,0x00,0x4E,0x00}, 7},
		{0x98, {0x3e,0x07}, 2},
		{0x35, {0}, 0},
		{0x21, {0}, 0},
		{0x11, {0}, 0}
	};

#if gc9a01a_BCKL == 15
	gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_SEL_15;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
#endif

	//Initialize non-SPI GPIOs
	gpio_set_direction(GC9A01A_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(GC9A01A_RST, GPIO_MODE_OUTPUT);
	
#if gc9a01a_ENABLE_BACKLIGHT_CONTROL
    gpio_set_direction(GC9A01A_BCKL, GPIO_MODE_OUTPUT);
#endif
	//Reset the display
	gpio_set_level(GC9A01A_RST, 1);
	vTaskDelay(5 / portTICK_RATE_MS);
	gpio_set_level(GC9A01A_RST, 0);
	vTaskDelay(20 / portTICK_RATE_MS);
	gpio_set_level(GC9A01A_RST, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	ESP_LOGI(TAG, "Initialization.");

	//Send all the commands
	uint16_t cmd = 0;
	while (ili_init_cmds[cmd].databytes!=0xff) {
		gc9a01a_send_cmd(ili_init_cmds[cmd].cmd);
		gc9a01a_send_data(ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes&0x1F);
		if (ili_init_cmds[cmd].databytes & 0x80) {
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		cmd++;
	}
	vTaskDelay(120 / portTICK_RATE_MS);
	
	gc9a01a_send_cmd(0x29);
	gc9a01a_enable_backlight(true);

    gc9a01a_set_orientation(CONFIG_LVGL_DISPLAY_ORIENTATION);

#if gc9a01a_INVERT_COLORS == 1
	//NOT IMPLEMENTED
#else
	//NOT IMPLEMENTED
#endif
}


void gc9a01a_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
	uint8_t data[4];

	/*Column addresses*/
	gc9a01a_send_cmd(0x2A);
	data[0] = (area->x1 >> 8) & 0xFF;
	data[1] = area->x1 & 0xFF;
	data[2] = (area->x2 >> 8) & 0xFF;
	data[3] = area->x2 & 0xFF;
	gc9a01a_send_data(data, 4);

	/*Page addresses*/
	gc9a01a_send_cmd(0x2B);
	data[0] = (area->y1 >> 8) & 0xFF;
	data[1] = area->y1 & 0xFF;
	data[2] = (area->y2 >> 8) & 0xFF;
	data[3] = area->y2 & 0xFF;
	gc9a01a_send_data(data, 4);

	/*Memory write*/
	gc9a01a_send_cmd(0x2C);


	uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

	gc9a01a_send_color((void*)color_map, size * 2);
}

void gc9a01a_enable_backlight(bool backlight)
{
#if GC9A01A_ENABLE_BACKLIGHT_CONTROL
    ESP_LOGI(TAG, "%s backlight.", backlight ? "Enabling" : "Disabling");
    uint32_t tmp = 0;

#if GC9A01A_BCKL_ACTIVE_LVL==1)
    tmp = backlight ? 1 : 0;
#else
    tmp = backlight ? 0 : 1;
#endif

    gpio_set_level(GC9A01A_BCKL, tmp);
#endif
}

void gc9a01a_sleep_in()
{
	//uint8_t data[] = {0x08};
	//gc9a01a_send_cmd(0x10);
	//gc9a01a_send_data(&data, 1);
}

void gc9a01a_sleep_out()
{
	//uint8_t data[] = {0x08};
	//gc9a01a_send_cmd(0x11);
	//gc9a01a_send_data(&data, 1);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void gc9a01a_send_cmd(uint8_t cmd)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(GC9A01A_DC, 0);	 /*Command mode*/
    disp_spi_send_data(&cmd, 1);
}

static void gc9a01a_send_data(void * data, uint16_t length)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(GC9A01A_DC, 1);	 /*Data mode*/
    disp_spi_send_data(data, length);
}

static void gc9a01a_send_color(void * data, uint32_t length)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(GC9A01A_DC, 1);   /*Data mode*/
    disp_spi_send_colors(data, length);
}

static void gc9a01a_set_orientation(uint8_t orientation)
{
	/* NOT IMPLEMENTED
    // ESP_ASSERT(orientation < 4);

    const char *orientation_str[] = {
        "PORTRAIT", "PORTRAIT_INVERTED", "LANDSCAPE", "LANDSCAPE_INVERTED"
    };

    ESP_LOGI(TAG, "Display orientation: %s", orientation_str[orientation]);

#if defined CONFIG_LVGL_PREDEFINED_DISPLAY_M5STACK
    uint8_t data[] = {0x68, 0x68, 0x08, 0x08};
#elif defined (CONFIG_LVGL_PREDEFINED_DISPLAY_WROVER4)
    uint8_t data[] = {0x4C, 0x88, 0x28, 0xE8};
#elif defined (CONFIG_LVGL_PREDEFINED_DISPLAY_NONE)
    uint8_t data[] = {0x48, 0x88, 0x28, 0xE8};
#endif

    ESP_LOGI(TAG, "0x36 command value: 0x%02X", data[orientation]);

    gc9a01a_send_cmd(0x36);
    gc9a01a_send_data((void *) &data[orientation], 1);
	*/
}
