
#include "SSD1306.h"
#include "bitmap.h"
#include "anime.h"

#include "OLED_vram.h"

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
	// digitalWrite(RES,0);
	delay(10);
	digitalWrite(RES,1);
	oled_setup();
}

void loop(void)
{
	// delay(1000);
	// Serial.println("");
	// Serial.println("wait input to draw next frame");
	// while(!Serial.available()) delay(50);
	// while(Serial.available()) Serial.read();
	// sample_draw();
	sample_draw_tween();
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
extern sBITMAP cube3_anime[];
extern sBITMAP num_tiny_array[];

extern sBITMAP n1_icon;
extern sBITMAP n2_icon;
extern sBITMAP n3_icon;
extern sBITMAP n4_icon;


int outQuart(float t, float b, float c, float d){
	if(t>d) return b+c;
	return -c * (pow(t / d - 1, 4) - 1) + b;
}

int outQuint(float t, float b, float c, float d){
	if(t>d) return b+c;
	return c * (pow(t / d - 1, 5) + 1) + b;
}

int outBack(float t, float b, float c, float d){
  t = t / d - 1;
  return c * (t * t * ((1.70158 + 1) * t + 1.70158) + 1) + b;
}


// local function calculatePAS(p,a,c,d)
//   p, a = p or d * 0.3, a or 0
//   if a < abs(c) then return p, c, p / 4 end -- p, a, s
//   return p, a, p / (2 * pi) * asin(c/a) -- p,a,s
// end
// local function outElastic(t, b, c, d, a, p)
//   local s
//   if t == 0 then return b end
//   t = t / d
//   if t == 1 then return b + c end
//   p,a,s = calculatePAS(p,a,c,d)
//   return a * pow(2, -10 * t) * sin((t * d - s) * (2 * pi) / p) + c + b
// end

int outElastic(float t, float b, float c, float d){
	float s,a;
	float p=d*0.3;
	if (t == 0){return b;}
	t = t / d;
	if (t == 1){return b + c;}
	a=c/4;
	s=p/4;
	return a * pow(2, -10 * t) * sin((t * d - s) * (2 * 3.141592653) / p) + c + b;
}

int outBounce(float t, float b, float c, float d){
	t = t / d;
	if (t < 1 / 2.75) {
		return c * (7.5625 * t * t) + b;
	}
	if (t < 2 / 2.75) {
		t = t - (1.5 / 2.75);
		return c * (7.5625 * t * t + 0.75) + b;
	} else if (t < 2.5 / 2.75) {
		t = t - (2.25 / 2.75);
		return c * (7.5625 * t * t + 0.9375) + b;
	}
	t = t - (2.625 / 2.75);
	return c * (7.5625 * t * t + 0.984375) + b;
}

void sample_draw_tween(void)
{
	sPOS pos={57,19};
	sPOS pos_next={57,19};
	unsigned long tic=millis();
	unsigned long t=millis()-tic;
	unsigned int frame;

	frame=0;
	pos_next.x=57;
	while(t<1000){
		t=millis()-tic;
		pos.y=outBack(t,19-OLED_HEIGHT,OLED_HEIGHT,1000);
		pos_next.y=pos.y+OLED_HEIGHT;
		oled_Draw(&pos_next,&n4_icon,REPLACE);
		oled_Draw(&pos,&n1_icon,REPLACE);
		oled_Update();
		delay(5);
		frame+=1;
	}
	Serial.print("Frame:");Serial.println(frame);
	delay(1000);


	tic=millis();t=0;
	frame=0;
	pos_next.y=19;
	while(t<1000){
		t=millis()-tic;
		pos.x=outQuint(t,57-OLED_WIDTH,OLED_WIDTH,1000);
		pos_next.x=pos.x+OLED_WIDTH;
		oled_Draw(&pos_next,&n1_icon,REPLACE);
		oled_Draw(&pos,&n2_icon,REPLACE);
		oled_Update();
		delay(5);
		frame+=1;
	}
	Serial.print("Frame:");Serial.println(frame);
	delay(1000);


	tic=millis();t=0;
	frame=0;
	pos_next.x=57;
	while(t<1000){
		t=millis()-tic;
		pos.y=outBounce(t,19-OLED_HEIGHT,OLED_HEIGHT,1000);
		pos_next.y=pos.y+OLED_HEIGHT;
		oled_Draw(&pos_next,&n2_icon,REPLACE);
		oled_Draw(&pos,&n3_icon,REPLACE);
		oled_Update();
		delay(5);
		frame+=1;
	}
	Serial.print("Frame:");Serial.println(frame);
	delay(1000);


	tic=millis();t=0;
	frame=0;
	pos_next.y=19;
	while(t<1000){
		t=millis()-tic;
		pos.x=outElastic(t,57-OLED_WIDTH,OLED_WIDTH,1000);
		pos_next.x=pos.x+OLED_WIDTH;
		oled_Draw(&pos_next,&n3_icon,REPLACE);
		oled_Draw(&pos,&n4_icon,REPLACE);
		oled_Update();
		delay(5);
		frame+=1;
	}
	Serial.print("Frame:");Serial.println(frame);
	delay(1000);
}

void sample_draw(void)
{
	static unsigned char i=0,j=0;
	static char ud=1,lr=1;
	static sPOS pos0={30,40};
	static sPOS pos1={7,7};
	static sPOS pos2={10,-16};
	// static sPOS pos2={0,0};
	static sPOS pos2_1={0,0};
	static sPOS pos2_2={0,0};
	static sPOS pos2_3={0,0};

	// oled_Draw(&pos0,&pDot,OR);
	// oled_Update();
	// pos1.y+=ud;
	// pos1.x+=lr;
	// if(pos1.x>=OLED_WIDTH-loading_anime[0].w)lr=-1;
	// if(pos1.y>=OLED_HEIGHT-loading_anime[0].h)ud=-1;
	// if(pos1.x<1)lr=1;
	// if(pos1.y<1)ud=1;

	// delay(30);
	// oled_Draw(&pos0,&pqr_code,REPLACE);
	// oled_Draw(&pos1,&loading_anime[i],ERASE);
	
	// oled_Draw(&pos0,&rolling_cat[i],REPLACE);
	
	// oled_Draw(&pos2,&cube3_anime[j],REPLACE);
	oled_Draw(&pos2,&anime_frame_group1[j],REPLACE);
	oled_Draw(&pos2_1,&anime_frame_group1[j],REPLACE);
	oled_Draw(&pos2_2,&anime_frame_group1[j],REPLACE);
	oled_Draw(&pos2_3,&anime_frame_group1[j],REPLACE);
	// i=i+1>7?0:i+1;
	j=j+1>9?0:j+1;

	Serial.print(pos2.x);
	Serial.print(" ");
	Serial.println(pos2.y);

	delay(50);
	pos2.x+=3;
	pos2.y+=1;
	if(pos2.x>OLED_WIDTH) pos2.x-=OLED_WIDTH;
	if(pos2.y>OLED_HEIGHT) pos2.y-=OLED_HEIGHT;

	if(pos2.x>OLED_WIDTH/2) pos2_1.x=pos2.x-OLED_WIDTH;
	else pos2_1.x=pos2.x+OLED_WIDTH;
	pos2_1.y=pos2.y;

	if(pos2.x>OLED_WIDTH/2) pos2_2.x=pos2.x-OLED_WIDTH;
	else pos2_2.x=pos2.x+OLED_WIDTH;
	if(pos2.y>OLED_HEIGHT/2) pos2_2.y=pos2.y-OLED_HEIGHT;
	else pos2_2.y=pos2.y+OLED_HEIGHT;

	pos2_3.x=pos2.x;
	if(pos2.y>OLED_HEIGHT/2) pos2_3.y=pos2.y-OLED_HEIGHT;
	else pos2_3.y=pos2.y+OLED_HEIGHT;

	// oled_Draw(&pos0,&num_tiny_array[i],REPLACE);
	// i=i+1>9?0:i+1;
	oled_Update();
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

void _oled_set_cursor(unsigned char x, unsigned char y){
	oled_set_cursor(x, y);
}

void _oled_datas(unsigned char *addr,unsigned char length){
	oled_datas(addr,length);
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





/***************************************************

unsigned char oled_buffer[OLED_HEIGHT>>3][OLED_WIDTH]={0};
unsigned char oled_vram[OLED_HEIGHT>>3][OLED_WIDTH]={0};

// draw to buffer
void oled_Draw(sPOS* pos,sBITMAP* bitmap,eBlendMode blendmode)
{
	short notAlign=pos->y;
	unsigned char offset=pos->y/8;
	short h_t;	// 剩余高度
	unsigned char mask;
	unsigned char vy,by,yh;
	short i,j,bline=0,sline=0;
	unsigned char bitmap_buffer[OLED_HEIGHT>>3][OLED_WIDTH]={0};
	sRECT vrect;	// 实际显示区域矩形
	sRECT brect={0,0,0,0};	// 位图显示区域矩形

	while(notAlign<0) notAlign+=256;
	notAlign=(unsigned short)(notAlign)&0x7;

	// 实际显示宽高
	#define max(a,b) ((a)>=(b)?(a):(b))
	#define min(a,b) ((a)<(b)?(a):(b))
	vrect.x=max(0,pos->x);
	vrect.y=max(0,pos->y);
	vrect.w=min(OLED_WIDTH,pos->x+bitmap->w)-vrect.x;
	vrect.h=min(OLED_HEIGHT,pos->y+bitmap->h)-vrect.y;
	if(pos->x<0)brect.x=bitmap->w-vrect.w;
	if(pos->y<0)brect.y=bitmap->h-vrect.h;
	brect.w=vrect.w;
	brect.h=vrect.h;
	#undef max(a,b)
	#undef min(a,b)
	// 在画布外
	if(vrect.w<=0 || vrect.h<=0) return;

	#define bitmap(x,y) bitmap->map[(x)+(y)*bitmap->w]

	vy=vrect.y;
	by=brect.y;

	while(vy<vrect.y+vrect.h){
		if(vy&0x7){		// $by should be 0 here
			yh=(8-vy&0x7);
			for (i = 0; i < vrect.w; ++i){
				bitmap_buffer[vy/8][i+vrect.x] |= bitmap(i+brect.x,by/8)<<(8-yh);
			}
			vy+=yh;
			by+=yh;
		}else if(by&0x7){	//$vy&0x7 should be 0 here
			yh=(8-by&0x7);
			for (i = 0; i < vrect.w; ++i){
				bitmap_buffer[vy/8][i+vrect.x] |= bitmap(i+brect.x,by/8)>>(8-yh);
			}
			vy+=yh;
			by+=yh;
		}else{
			yh=8;
			for (i = 0; i < vrect.w; ++i){
				bitmap_buffer[vy/8][i+vrect.x] = bitmap(i+brect.x,by/8);
			}
			vy+=8;
			by+=8;
		}
	}

	#undef bitmap(x,y)

	Serial.print("yyyyyy=");
	Serial.print(pos->y);Serial.print(" ");
	Serial.println(brect.y);

	h_t=vrect.h;
	notAlign=(unsigned short)(vrect.y)&0x7;
	for (j=vrect.y/8; ; j++) {
		if(notAlign && j==vrect.y/8) {
			mask = 0xFF<<notAlign;		// 底端对齐
			if(h_t>=8-notAlign){
				h_t-=(8-notAlign);
			}else{
				mask &= 0xFF>>(8-notAlign-h_t);
				h_t=0;
			}
		} else if(h_t<8) {
			mask = 0xFF>>(8-h_t);
			h_t=0;
		} else {
			mask = 0xFF;
			h_t-=8;
		}
		switch(blendmode){
			default:
			case REPLACE:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] = (oled_buffer[j][i]&~mask) | (bitmap_buffer[j][i]&mask);
				}
				Serial.print("j=");Serial.print(j);Serial.print(" ");Serial.print("mask=");Serial.print(mask,HEX);Serial.println();
			break;
			case OR:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] |= bitmap_buffer[j][i]&mask;
				}
			break;
			case ERASE:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] &= (~bitmap_buffer[j][i]);
				}
			break;
			case XOR:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] ^= bitmap_buffer[j][i]&mask;
				}
			break;
			case AND:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] &= (oled_buffer[j][i]&~mask) | (bitmap_buffer[j][i]&mask);
				}
			break;
			case NOT:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] |= (~bitmap_buffer[j][i])&mask;
				}
			break;
			case XNOR:
				for (i=vrect.x; i<vrect.x+vrect.w; i++){
					oled_buffer[j][i] = ~(oled_buffer[j][i]^(bitmap_buffer[j][i]&mask));
				}
			break;
		}
		if(h_t==0)break;
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

// clear buffer
void buffer_clear()
{
	unsigned int x,y;
	for(y=0;y<OLED_HEIGHT>>3;y++){
		for(x = 0; x < OLED_WIDTH; x++){
			oled_buffer[y][x]=0;
		}
	}
}

// clear vram
void vram_clear()
{
	unsigned int x,y;
	for(y=0;y<OLED_HEIGHT>>3;y++){
		for (x = 0; x < OLED_WIDTH; x++) {
			oled_vram[y][x]=0;
		}
	}
}

// update vram to OLED screen
void oled_Update()
{
	unsigned char i,x=0,y=0;
	unsigned char jump_start,jump_to;
	unsigned int jump;

	Serial.println("update###############");
	Serial.println("");

	for(y=0;y<OLED_HEIGHT>>3;y++){
		x=0;
		while(x<OLED_WIDTH){
			jump = oled_vram_buffer_next_jump(x,y);
			jump_start = (unsigned char)(jump>>8);
			jump_to = (unsigned char)(jump&0xFF);

			if(x!=jump_start){
				for (i = x; i < jump_start && i<OLED_WIDTH-1; ++i) {
					oled_vram[y][i] = oled_buffer[y][i];
				}
				jump_start = jump_start>OLED_WIDTH?OLED_WIDTH:jump_start;
				_oled_set_cursor(x,y);
				_oled_datas(&(oled_vram[y][x]),jump_start-x);
			}
			if(jump_to>=OLED_WIDTH) break;
			x = jump_to;
		}
	}
	// screen_update();
	buffer_clear();
}
*/
