
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


#endif
