
#ifndef BITMAP_H
#define BITMAP_H

typedef struct SPOS
{
	short x;
	short y;
}sPOS;

typedef struct SBITMAP
{
	unsigned char w;
	unsigned char h;
	unsigned char* map;
}sBITMAP;

typedef struct SRECT
{
	short x;
	short y;
	short w;
	short h;
}sRECT;

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
