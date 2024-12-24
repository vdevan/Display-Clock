#line 1 "C:\\Users\\vdeva\\My Drive\\Work\\Github\\Displayclock\\ver1.03\\DisplayClock\\Weather8x8px.h"
//Use the URL http://javl.github.io/image2cpp/ to convert bitmap for Adafruit Graphics
//

const unsigned char RainBmp[] PROGMEM = {
	0x3E,0x41,0xc3,0x76,0x3d,0x55,0x55,0x44
};

const unsigned char CloudBmp[] PROGMEM = {
	0x3e, 0xc1, 0x83, 0x76, 0x3c, 0x00, 0x00, 0x00
};

const unsigned char Waning[] PROGMEM = {
	0x3c,0x42,0x81,0x83,0x87,0x7e,0x3c,0x00

};

const unsigned char Waxing[] PROGMEM = {
	0x3c,0x7e,0xfd,0xf9,0xf1,0x42,0x3c,0x00
};

const unsigned char FullMoon[] PROGMEM = {
	0x3c,0x7e,0xff,0xff,0xff,0x7e,0x3c,0x00

};
const unsigned char NewMoon[] PROGMEM = {
	0x3c,0x42,0x81,0x81,0x81,0x42,0x3c,0x00
};

const unsigned char HumidityBmp[] PROGMEM = {
0x10, 0x28, 0x44, 0x3c, 0x20, 0x92, 0x95, 0x67
};

const unsigned char WindSpeed[] PROGMEM = {
0x60, 0xb1, 0x0e, 0x60, 0xb1, 0x0e, 0x61, 0xb8
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 32)
const int bitmapArrayLen = 8;

const unsigned char* bitmapArray[8] = {
	Waning,Waxing,FullMoon,NewMoon,RainBmp,CloudBmp,HumidityBmp,WindSpeed
};
