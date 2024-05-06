/**
 * Copyright (c) 2023 Milk-V
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>
//#include <unistd.h>
#include <stdint.h>

#include <wiringx.h>

#include "font.h"
#include "ssd1306.h"
 
static int fd_i2c;

void ssd1306_write_byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		wiringXI2CWriteReg8(fd_i2c, 0x40, dat);
	}
	else
	{
		wiringXI2CWriteReg8(fd_i2c, 0x00, dat);
	}
}

void ssd1306_set_position(unsigned char x, unsigned char y) 
{
 	ssd1306_write_byte(YLevel+y,OLED_CMD);
	ssd1306_write_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	ssd1306_write_byte((x&0x0f),OLED_CMD); 
}  

void ssd1306_clear_full_screen(unsigned dat)  
{ 
	uint8_t i,n,color;	
	if(dat)
	{
		color = 0xff;
	}
	else
	{
		color = 0;
	}
	for(i=0;i<(PAGE_SIZE/2);i++)  
	{  
		ssd1306_write_byte(YLevel+i,OLED_CMD);
		ssd1306_write_byte(XLevelL,OLED_CMD);
		ssd1306_write_byte(XLevelH,OLED_CMD);
		for(n=0;n<WIDTH;n++)
		{
			ssd1306_write_byte(color,OLED_DATA); 
		}
	}
}

void ssd1306_init(void)
{
	ssd1306_write_byte(0xAE,OLED_CMD); /*display off*/
	ssd1306_write_byte(0x00,OLED_CMD); /*set lower column address*/ 
	ssd1306_write_byte(0x10,OLED_CMD); /*set higher column address*/
	ssd1306_write_byte(0x00,OLED_CMD); /*set display start line*/ 
	ssd1306_write_byte(0xB0,OLED_CMD); /*set page address*/ 
	ssd1306_write_byte(0x81,OLED_CMD); /*contract control*/ 
	ssd1306_write_byte(0xff,OLED_CMD); /*128*/ 
	ssd1306_write_byte(0xA1,OLED_CMD); /*set segment remap*/ 
	ssd1306_write_byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	ssd1306_write_byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	ssd1306_write_byte(0x1F,OLED_CMD); /*duty = 1/32*/ 
	ssd1306_write_byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	ssd1306_write_byte(0xD3,OLED_CMD); /*set display offset*/ 
	ssd1306_write_byte(0x00,OLED_CMD); 
	ssd1306_write_byte(0xD5,OLED_CMD); /*set osc division*/ 
	ssd1306_write_byte(0x80,OLED_CMD); 
	ssd1306_write_byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	ssd1306_write_byte(0x1f,OLED_CMD); 
	ssd1306_write_byte(0xDA,OLED_CMD); /*set COM pins*/ 
	ssd1306_write_byte(0x00,OLED_CMD); 
	ssd1306_write_byte(0xdb,OLED_CMD); /*set vcomh*/ 
	ssd1306_write_byte(0x40,OLED_CMD); 
	ssd1306_write_byte(0x8d,OLED_CMD); /*set charge pump enable*/ 
	ssd1306_write_byte(0x14,OLED_CMD);
	ssd1306_clear_full_screen(0);
	ssd1306_write_byte(0xAF,OLED_CMD); /*display ON*/ 
}  

void ssd1306_push_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t sizey)
{      	
	uint8_t c=0,sizex=sizey/2;
	uint16_t i=0,size1;
	if(sizey==8)size1=6;
	else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
	c=chr-' ';
	ssd1306_set_position(x,y);
	for(i=0;i<size1;i++)
	{
		if(i%sizex==0&&sizey!=8) ssd1306_set_position(x,y++);
		if(sizey==8) ssd1306_write_byte(asc2_0806[c][i],OLED_DATA);
		else if(sizey==16) ssd1306_write_byte(asc2_1608[c][i],OLED_DATA);
		else return;
	}
}

void ssd1306_push_string(uint8_t x,uint8_t y,uint8_t *chr,uint8_t sizey)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		
		ssd1306_push_char(x,y,chr[j++],sizey);
		if(sizey==8)x+=6;
		else x+=sizey/2;
	}
}

void ssd1306_i2c_init() 
{
    if ((fd_i2c = wiringXI2CSetup(I2C_DEV_OLED, IIC_SLAVE_ADDR)) <0) {
        printf("I2C Setup failed: %i\n", fd_i2c);
    }

    ssd1306_init();
    ssd1306_clear_full_screen(0);
	
}