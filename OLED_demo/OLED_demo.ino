
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
	Serial.println("...tic...");
	delay(1000);
	sample_draw();
}

extern sBITMAP pTest1;
extern sBITMAP pTest2;
extern sBITMAP pTest3;
extern sBITMAP pA64;
void sample_draw(void)
{
	static unsigned char i=0;
	sPOS pos0={0,0};
	switch(i){
		default: i=0;
		case 0: oled_Clear();oled_draw(&pos0,&pTest1); break;
		case 1: oled_draw(&pos0,&pTest2); break;
		case 2: oled_draw(&pos0,&pTest3); break;
		case 3: oled_draw(&pos0,&pA64); break;
	}
	i++;
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

void oled_draw(sPOS* pos,sBITMAP* bitmap)
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
	u8 i,n;
	for(i=0;i<8;i++) {
		oled_command(0xb0+i);    //设置页地址（0~7）
		oled_command(0x00);      //设置显示位置―列低地址
		oled_command(0x10);      //设置显示位置―列高地址   
		for(n=0;n<128;n++)oled_data(0); 
	}
}

void oled_set_cursor(unsigned char x, unsigned char y) 
{ 
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

void oled_command(unsigned char cmd)
{
	digitalWrite(DC, 0);
	digitalWrite(CS, 0);
	shiftOut(D1, D0, MSBFIRST, cmd);
	// soft_spi_wr(cmd);
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
