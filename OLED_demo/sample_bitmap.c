
#include "bitmap.h"


const unsigned char test1[2][16]={
	{0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
	{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
};

const unsigned char test2[2][16]={
	{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},
	{0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},
};

const unsigned char test3[0x2][0x10]={
	{0x00,0x00,0x04,0x3E,0x24,0x24,0x00,0x00,0x00,0x3E,0x2A,0x2A,0x2E,0x00,0x00,0x00},
	{0x00,0x00,0x2E,0x2A,0x2A,0x3A,0x00,0x00,0x00,0x04,0x3E,0x24,0x24,0x00,0x00,0x00}
};

const unsigned char a64[0x8][0x40]={
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x1E,0x00,0x00,0x00,0x80,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x80,0x7C,0x03,0x02,0x02,0x02,0x02,0x01,0x01,0x81,0x7D,0x07,0x00,0xF8,0x8C,0x24,0x14,0x14,0x08,0x80,0x00,0xF8,0x07,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0xF0,0x08,0x08,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x02,0x02,0x01,0x00,0x03,0x02,0x02,0x00,0x00,0x03,0x82,0x03,0x00,0x00,0x00,0x00,0x01,0x02,0x82,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x1F,0xE0,0x80,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0xC0,0x20,0x10,0x10,0x10,0x20,0xC0,0x00,0xF0,0x20,0x20,0x10,0x00,0x00,0xFF,0x00,0x00,0x00,0xC0,0x20,0x20,0x20,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x06,0x01,0x00,0x03,0x04,0x02,0x01,0x00,0x00,0x00,0x01,0x02,0x04,0x04,0x04,0x02,0x01,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x07,0x04,0x04,0x00,0x07,0x04,0x04,0x02,0x05,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0xFC,0x84,0x04,0x04,0x04,0x04,0x00,0x00,0xFC,0x44,0xBC,0x00,0xFC,0x24,0x24,0x24,0x24,0x24,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x44,0x44,0x44,0x7C,0x00,0x7C,0x40,0x40,0x40,0xFC,0x00,0x00,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x7F,0x04,0x03,0x00,0x7F,0x41,0x23,0x05,0x09,0x15,0x21,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

const unsigned char dot[0x1][0x2]={
	{0x01,0x01}
};



sBITMAP pTest1={16,16,test1};
sBITMAP pTest2={16,16,test2};
sBITMAP pTest3={16,16,test3};
sBITMAP pA64={64,64,a64};
sBITMAP pDot={1,1,dot};
