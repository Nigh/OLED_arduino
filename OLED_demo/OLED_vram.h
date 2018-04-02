
#ifndef _OLED_VRAM_H_
#define _OLED_VRAM_H_

#include "bitmap.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define MIN_SAME_SIZE 4


void oled_Draw(sPOS* pos,sBITMAP* bitmap,eBlendMode blendmode);
void oled_Update(void);


extern void _oled_set_cursor(unsigned char x, unsigned char y);
extern void _oled_datas(unsigned char *addr,unsigned char length);

// examples for function above

// void oled_datas(unsigned char* data,unsigned char length)
// {
// 	digitalWrite(DC, 1);
// 	digitalWrite(CS, 0);
// 	while(length-->0){
// 		shiftOut(D1, D0, MSBFIRST, *data++);
// 	}
// 	digitalWrite(CS, 1);
// }

// void oled_set_cursor(unsigned char x, unsigned char y) 
// {
// 	oled_command(0xb0+y);
// 	oled_command((x&0x0f));
// 	oled_command((x>>4)|0x10);
// }

#endif
