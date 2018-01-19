
#ifndef BITMAP_H
#define BITMAP_H

typedef struct SPOS
{
	unsigned char x;
	unsigned char y;
}sPOS;

typedef struct SBITMAP
{
	unsigned char w;
	unsigned char h;
	unsigned char* map;
}sBITMAP;

typedef enum BLENDMODE
{
	REPLACE,
	OR,
	ERASE,
	AND,
	NOT,
	XOR,	// 异或
	XNOR,	// 同或
}eBlendMode;

#endif
