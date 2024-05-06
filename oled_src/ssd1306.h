#ifndef __SSD1306_H__
#define __SSD1306_H__

 /* Example code to SSD1306 OLED Display Module

    NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Duo
    GPIO (and therefore I2C) cannot be used at 5v.

    You will need to use a level shifter on the I2C lines if you want to run the
    board at 5v.

    Please wire according to the notes below and make sure 
    the pins are set for the correct function before running the program.

    I2C1_SDA -> SDA on SSD1306 Moudle
    I2C1_SCL -> SCL on SSD1306 Moudle
    3.3v -> VCC on SSD1306 Moudle
    GND -> GND on SSD1306 Moudle
 */

//The I2C used
#define I2C_DEV_OLED "/dev/i2c-0"

//Device address
#define IIC_SLAVE_ADDR 0x3C

#define OLED_CMD     0
#define OLED_DATA    1

#define PAGE_SIZE    8
#define XLevelL		 0x00
#define XLevelH		 0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     32


/**
  * @brief          SSD1306 initialization
  * @retval         void
  */
void ssd1306_init(void);

/**
  * @brief          SSD1306 I2C initialization
  * @retval         void
  */
void ssd1306_i2c_init(void);

/**
  * @brief          Write one byte of data to SSD1306
  * @param[in]      dat: Data
  * @param[in]      cmd: Mark whether dat is a command or data
  * @retval         void
  */
void ssd1306_write_byte(unsigned dat,unsigned cmd);

/**
  * @brief          Set brush position
  * @param[in]      x: Horizontal coordinate position
  * @param[in]      y: Vertical coordinate position
  * @retval         void
  */
void ssd1306_set_position(unsigned char x, unsigned char y);

/**
  * @brief          Clear the display on the entire screen
  * @param[in]      dat: Color to be filled
  * @retval         void
  */
void ssd1306_clear_full_screen(unsigned dat);


/**
  * @brief          Display a character on the screen
  * @param[in]      x: Horizontal coordinate position
  * @param[in]      y: Vertical coordinate position
  * @param[in]      chr: Characters to display
  * @param[in]      sizey: Font size
  * @retval         void
  */
void ssd1306_push_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t sizey);

/**
  * @brief          Display string on screen
  * @param[in]      x: Horizontal coordinate position
  * @param[in]      y: Vertical coordinate position
  * @param[in]      chr: Pointer to the string to display
  * @param[in]      sizey: Font size
  * @retval         void
  */
void ssd1306_push_string(uint8_t x,uint8_t y,uint8_t *chr,uint8_t sizey);





#endif