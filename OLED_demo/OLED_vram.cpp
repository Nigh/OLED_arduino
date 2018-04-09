
#include "OLED_vram.h"

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
			for (i = 0; i < vrect.w; ++i){bitmap_buffer[vy/8][i+vrect.x] |= bitmap(i+brect.x,by/8)<<(8-yh); }
			vy+=yh; by+=yh;
		}else if(by&0x7){	//$vy&0x7 should be 0 here
			yh=(8-by&0x7);
			for (i = 0; i < vrect.w; ++i){bitmap_buffer[vy/8][i+vrect.x] |= bitmap(i+brect.x,by/8)>>(8-yh); }
			vy+=yh; by+=yh;
		}else{
			yh=8;
			for (i = 0; i < vrect.w; ++i){bitmap_buffer[vy/8][i+vrect.x] = bitmap(i+brect.x,by/8); }
			vy+=8; by+=8;
		}
	}

	#undef bitmap(x,y)

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

