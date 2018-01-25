
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

extern sBITMAP loading_anime[];
extern sBITMAP rolling_cat[];
extern sBITMAP anime_frame_group1[];
extern sBITMAP pTest1;
extern sBITMAP pTest2;
extern sBITMAP pTest3;
extern sBITMAP pA64;
extern sBITMAP pDot;
extern sBITMAP pqr_code;
void sample_draw(void)
{
	static unsigned char i=0;
	static char ud=1,lr=1;
	static sPOS pos0={1,1};
	static sPOS pos1={7,7};
	// oled_Draw(&pos0,&pDot,OR);
	// oled_Update();
	// pos1.y+=ud;
	// pos1.x+=lr;
	// if(pos1.x>=OLED_WIDTH-loading_anime[0].w)lr=-1;
	// if(pos1.y>=OLED_HEIGHT-loading_anime[0].h)ud=-1;
	// if(pos1.x<1)lr=1;
	// if(pos1.y<1)ud=1;

	delay(60);
	oled_Draw(&pos0,&pqr_code,REPLACE);
	oled_Draw(&pos1,&loading_anime[i],ERASE);
	// oled_Draw(&pos0,&rolling_cat[i],REPLACE);
	oled_Update();
	i=i+1>7?0:i+1;
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
	unsigned char vw,vh;
	unsigned char mask;
	unsigned char i,j;
	unsigned char bitmap_buffer[OLED_HEIGHT>>3][OLED_WIDTH]={0};

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

	// 实际显示宽高
	vw=pos->x+bitmap->w>OLED_WIDTH?(OLED_WIDTH-pos->x):(bitmap->w);
	vh=pos->y+bitmap->h>OLED_HEIGHT?(OLED_HEIGHT-pos->y):(bitmap->h);
	unsigned char h_t=vh;	// 剩余高度

	for (j=pos->y>>3; ; j++) {
		if(notAlign && j==pos->y>>3) {
			mask = 0xFF<<notAlign;
			if(h_t<8) {
				mask &= 0xFF>>(7-h_t);
				h_t = 0;
			} else {
				h_t-=(8-notAlign);
			}

		} else if(h_t<8) {
			mask = 0xFF>>(8-h_t);
			h_t=0;
		} else {
			mask = 0xFF;
			h_t-=8;
		}
		switch(blendmode){
			case REPLACE:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] = (oled_buffer[j][i]&~mask) | (bitmap_buffer[j][i]&mask);
				}
			break;
			case OR:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] |= bitmap_buffer[j][i]&mask;
				}
			break;
			case ERASE:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] &= (~bitmap_buffer[j][i]);
				}
			break;
			case XOR:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] ^= bitmap_buffer[j][i]&mask;
				}
			break;
			case AND:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] &= (oled_buffer[j][i]&~mask) | (bitmap_buffer[j][i]&mask);
				}
			break;
			case NOT:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] |= (~bitmap_buffer[j][i])&mask;
				}
			break;
			case XNOR:
				for (i=pos->x; i<pos->x+vw; i++){
					oled_buffer[j][i] = ~(oled_buffer[j][i]^(bitmap_buffer[j][i]&mask));
				}
			break;
		}
		if(h_t==0)break;
	}
}

#define MIN_SAME_SIZE 4
// 高位是跳跃起点，低位是跳跃终点
unsigned int oled_vram_buffer_next_jump(unsigned char x,unsigned char y){
	unsigned int i;
	unsigned char same_count=0;
	for(i=x;i<OLED_WIDTH;i++){
		if(oled_vram[y][i]==oled_buffer[y][i]){
			same_count+=1;
		}else{
			if(same_count>MIN_SAME_SIZE){
				return ((i-same_count)<<8)|(i&0xFF);
			}
			same_count=0;
		}
	}
    if(same_count>MIN_SAME_SIZE){
        return ((i-same_count)<<8)|(i&0xFF);
    }
	return 0xFFFF;
}

// update vram to OLED screen
void oled_Update()
{
	unsigned char i,x=0,y=0;
	unsigned char jump_start,jump_to;
	unsigned int jump;

	for(y=0;y<OLED_HEIGHT>>3;y++){
		x=0;
		while(x<OLED_WIDTH){
			jump = oled_vram_buffer_next_jump(x,y);
			jump_start = (unsigned char)(jump>>8);
			jump_to = (unsigned char)(jump&0xFF);

            // printf("x=%d,y=%d,jump_start=%d,jump_to=%d\n",x,y,jump_start,jump_to);

			if(x!=jump_start){
				for (i = x; i < jump_start && i<OLED_WIDTH-1; ++i) {
                    // printf("oled_vram[%d][%d] = oled_buffer[%d][%d] = %02x\n",y,i,y,i,oled_buffer[y][i]);
					oled_vram[y][i] = oled_buffer[y][i];
				}
                // printf("oled_datas(&(oled_vram[%d][%d]),%d)\n",y,x,jump_start-x);
				oled_set_cursor(x,y);
				jump_start = jump_start>OLED_WIDTH?OLED_WIDTH:jump_start;
				oled_datas(&(oled_vram[y][x]),jump_start-x);
			}
			if(jump_to>=OLED_WIDTH) break;
			// oled_set_cursor(jump_to,y);
			x = jump_to;
		}
	}
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
	// x+=1;
	oled_command(0xb0+y);
	// oled_command((x&0x0f)|0x01);
	oled_command((x&0x0f));
	oled_command((x>>4)|0x10);
	// Serial.print("set xy=");
	// Serial.print(x);
	// Serial.print(',');
	// Serial.println(y);
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
	while(length-->0){
		// Serial.print(*data,HEX);
		// Serial.print(',');
		shiftOut(D1, D0, MSBFIRST, *data++);
	}
	// Serial.print("\n");
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
