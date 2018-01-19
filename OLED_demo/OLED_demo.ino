
#include "SSD1306.h"
#include "bitmap.h"
#include "anime.h"

#define GND	
#define VCC	13
#define D0	12	// for clock
#define D1	11	// for data
#define RES	10	// reset
#define DC	9	// data/command selector
#define CS	8	// enable

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void setup(void)
{
	Serial.begin(115200);
	Serial.println("Start");
	pinMode(VCC,OUTPUT);
	digitalWrite(VCC,1);
	pinMode(D0,OUTPUT);
	pinMode(D1,OUTPUT);
	pinMode(RES,OUTPUT);
	pinMode(DC,OUTPUT);
	pinMode(CS,OUTPUT);
	digitalWrite(D0,1);
	digitalWrite(D1,1);
	digitalWrite(DC,1);
	digitalWrite(CS,1);
	digitalWrite(RES,1);
	delay(1);
	digitalWrite(RES,0);
	delay(10);
	digitalWrite(RES,1);
	oled_setup();
}

void loop(void)
{
	// delay(1000);
	sample_draw();
}

extern sBITMAP pTest1;
extern sBITMAP pTest2;
extern sBITMAP pTest3;
extern sBITMAP pA64;
extern sBITMAP pDot;
void sample_draw(void)
{
	static unsigned char i=0;
	static char ud=1,lr=1;
	static sPOS pos0={1,1};
	// switch(i){
	// 	default: i=0;
	// 	case 0: oled_Clear();oled_draw_bitmap(&pos0,&pTest1); break;
	// 	case 1: oled_draw_bitmap(&pos0,&pTest2); break;
	// 	case 2: oled_draw_bitmap(&pos0,&pTest3); break;
	// 	// case 3: oled_draw_bitmap(&pos0,&pA64); break;
	// }
	// i++;
	Serial.print("x=");
	Serial.print(pos0.x);
	Serial.print(" y=");
	Serial.println(pos0.y);
	oled_Draw(&pos0,&pDot,OR);
	oled_Update();
	pos0.y+=ud;
	pos0.x+=lr;
	if(pos0.x>=OLED_WIDTH-2)lr=-1;
	if(pos0.y>=OLED_HEIGHT-2)ud=-1;
	if(pos0.x<=1)lr=1;
	if(pos0.y<=1)ud=1;
}


void oled_setup(void)
{
	oled_command(SSD1306_DISPLAYOFF);               // 0xAE
	oled_command(SSD1306_SETDISPLAYCLOCKDIV);       // 0xD5
	oled_command(0x80);                             // the suggested ratio 0x80
	oled_command(SSD1306_SETMULTIPLEX);             // 0xA8
	oled_command(0x3F);
	oled_command(SSD1306_SETDISPLAYOFFSET);         // 0xD3
	oled_command(0x0);                              // no offset
	oled_command(SSD1306_SETSTARTLINE | 0x0);       // line #0
	oled_command(SSD1306_CHARGEPUMP);               // 0x8D
	oled_command(0x14);
	oled_command(SSD1306_MEMORYMODE);               // 0x20
	oled_command(0x00);                             // 0x0 act like ks0108
	oled_command(SSD1306_SEGREMAP | 0x1);
	oled_command(SSD1306_COMSCANDEC);
	oled_command(SSD1306_SETCOMPINS);               // 0xDA
	oled_command(0x12);
	oled_command(SSD1306_SETCONTRAST);              // 0x81
	oled_command(0xCF);
	oled_command(SSD1306_SETPRECHARGE);             // 0xd9
	oled_command(0xF1);
	oled_command(SSD1306_SETVCOMDETECT);            // 0xDB
	oled_command(0x40);
	oled_command(SSD1306_DISPLAYALLON);
	oled_command(SSD1306_NORMALDISPLAY);            // 0xA6

	oled_On();
	delay(1000);
	oled_Off();
	delay(200);
	oled_On();
	oled_command(SSD1306_DISPLAYALLON_RESUME);      // 0xA4
	oled_Clear();
}

unsigned char oled_buffer[OLED_HEIGHT>>3][OLED_WIDTH]={0};
unsigned char oled_vram[OLED_HEIGHT>>3][OLED_WIDTH]={0};


// draw to buffer
void oled_Draw(sPOS* pos,sBITMAP* bitmap,eBlendMode blendmode)
{
	unsigned char notAlign=pos->y&0x7;
	unsigned char offset=pos->y>>3;
	unsigned char i,j;
	unsigned char bitmap_buffer[OLED_HEIGHT>>3][OLED_WIDTH]={0};

	unsigned char mask[OLED_HEIGHT>>3][OLED_WIDTH]={0};	// pixel mask

	// generate mask
	// pos->y
	// bitmap->w
	// bitmap->h

	if(notAlign){
		for (j = 0; j < 1+1+((bitmap->h-1)>>3) && j+offset<OLED_HEIGHT>>3; ++j) {
			for (i = 0; i < bitmap->w && pos->x+i<OLED_WIDTH; ++i){
				if(j>0){
					bitmap_buffer[offset+j][pos->x+i] |= bitmap->map[i+(j-1)*bitmap->w]>>(8-notAlign);
				}
				if(j < 1+((bitmap->h-1)>>3)){
					bitmap_buffer[offset+j][pos->x+i] |= bitmap->map[i+j*bitmap->w]<<notAlign;
				}
			}
		}
	}else{
		for (j = 0; j < 1+((bitmap->h-1)>>3) && j+offset<OLED_HEIGHT>>3; ++j) {
			for (i = 0; i < bitmap->w && pos->x+i<OLED_WIDTH; ++i) {
				bitmap_buffer[offset+j][pos->x+i] |= bitmap->map[i+j*bitmap->w];
			}
		}
	}
	switch(blendmode){
		case REPLACE:
		break;
		case OR:
		for (j = 0; j < OLED_HEIGHT>>3; ++j) {
			for (i = 0; i < OLED_WIDTH; ++i) {
				oled_buffer[j][i] |= bitmap_buffer[j][i];
			}
		}
		break;
	}
}

// 高位是跳跃起点，低位是跳跃终点
unsigned int oled_vram_buffer_next_jump(unsigned char x,unsigned char y){
	unsigned int i;
	unsigned char same_count=0;
	for(i=x;i<OLED_WIDTH;i++){
		if(oled_vram[y][i]==oled_buffer[y][i]){
			same_count+=1;
		}else{
			if(same_count>5){
				return ((i-same_count)<<8)|(i&0xFF);
			}
			same_count=0;
		}
	}
	return 0xFFFF;
}

// update vram to OLED screen
void oled_Update()
{
	unsigned char x=0,y=0;
	unsigned char jump_start,jump_to;
	unsigned int jump;
	for(y=0;y<OLED_HEIGHT>>3;y++){
		oled_set_cursor(x,y);
		for (x= 0; x < OLED_WIDTH; ++x) {
			oled_vram[y][x] = oled_buffer[y][x];
			// oled_data(oled_vram[y][x]);
		}
		oled_datas(oled_vram[y],OLED_WIDTH);
	}
	// for(y=0;y<OLED_HEIGHT>>3;y++){
	// 	x=0;
	// 	Serial.print("Y");
	// 	while(x<OLED_WIDTH){
	// 		jump = oled_vram_buffer_next_jump(x,y);
	// 		jump_start = (unsigned char)(jump>>8);
	// 		jump_to = (unsigned char)(jump&0xFF);

	// 		if(x!=jump_start){
	// 			oled_set_cursor(x,y);
	// 		}

	// 		while(x!=jump_start){
	// 			oled_vram[y][x] = oled_buffer[y][x];
	// 			oled_data(oled_vram[y][x]);
	// 			if(x++>=OLED_WIDTH) break;
	// 		}
	// 		if(x>=OLED_WIDTH) break;
	// 		oled_set_cursor(jump_to,y);
	// 		x = jump_to;
	// 	}
	// }
}

void oled_draw_bitmap(sPOS* pos,sBITMAP* bitmap)
{
	unsigned int j=0;
	unsigned char x,y;
	unsigned char row=bitmap->h&0x7?(bitmap->h>>3)+1:(bitmap->h>>3);
	for(y=pos->y>>3;y<row;y++) {
		oled_set_cursor(pos->x,y);
		for(x=0;x<bitmap->w;x++) oled_data(bitmap->map[j++]);
	}
}

void oled_Clear(void)
{
	unsigned char i,n,length;
	for(i=0;i<8;i++) {
		oled_command(0xb0+i);    //设置页地址（0~7）
		oled_command(0x00);      //设置显示位置―列低地址
		oled_command(0x10);      //设置显示位置―列高地址   
		digitalWrite(DC, 1);
		digitalWrite(CS, 0);
		length=128;
		while(length-->0) shiftOut(D1, D0, MSBFIRST, 0);
		digitalWrite(CS, 1);

	}
}

void oled_set_cursor(unsigned char x, unsigned char y) 
{
	// static unsigned char cmd[]={0xb0+y+2,((x&0xf0)>>4)|0x10,(x&0x0f)|0x01};
	// oled_cmds(cmd,3);
	oled_command(0xb0+y);
	oled_command(((x&0xf0)>>4)|0x10);
	oled_command((x&0x0f)|0x01);
}

void oled_On(void)
{
	oled_command(SSD1306_CHARGEPUMP);	// 0x8D
	oled_command(0x14);
	oled_command(SSD1306_DISPLAYON);	//--turn on oled panel
}

void oled_Off(void)
{
	oled_command(SSD1306_CHARGEPUMP);	// 0x8D
	oled_command(0x10);
	oled_command(SSD1306_DISPLAYOFF);	// 0xAE
}

void oled_data(unsigned char data)
{
	digitalWrite(DC, 1);
	digitalWrite(CS, 0);
	shiftOut(D1, D0, MSBFIRST, data);
	// soft_spi_wr(data);
	digitalWrite(CS, 1);
}

void oled_datas(unsigned char* data,unsigned char length)
{
	digitalWrite(DC, 1);
	digitalWrite(CS, 0);
	while(length-->0) shiftOut(D1, D0, MSBFIRST, *data++);
	// soft_spi_wr(data);
	digitalWrite(CS, 1);
}

void oled_command(unsigned char cmd)
{
	digitalWrite(DC, 0);
	digitalWrite(CS, 0);
	shiftOut(D1, D0, MSBFIRST, cmd);
	digitalWrite(CS, 1);
	digitalWrite(DC, 1);
}

void oled_cmds(unsigned char* cmd,unsigned char length)
{
	digitalWrite(DC, 0);
	digitalWrite(CS, 0);
	while(length-->0) shiftOut(D1, D0, MSBFIRST, *cmd++);
	digitalWrite(CS, 1);
	digitalWrite(DC, 1);
}

unsigned char soft_spi_wr(unsigned char byte)
{
	for (int i = 0; i < 8; ++i)
	{
		if((byte&0x80)>0) digitalWrite(D1,1);
		else digitalWrite(D1,0);
		digitalWrite(D0,0);
		digitalWrite(D0,1);
		byte<<=1;
	}
}
