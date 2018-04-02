
#include "OLED_vram.h"

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

			mask = 0xFF<<notAlign;		// 底端对齐
			if(h_t>=8-notAlign){
				h_t-=(8-notAlign);
			}else{
				mask &= 0xFF>>(8-notAlign-h_t);
				h_t=0;
			}
			// if(h_t<8) {
			// 	mask &= 0xFF>>(8-notAlign-h_t);
			// 	h_t = 0;
			// } else {
			// 	h_t-=(8-notAlign);
			// }
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

