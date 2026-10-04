#ifndef IMAGE_H
#define IMAGE_H

#include "gamestring.h"

unsigned char *LoadImage( string & fileName, int & width, int & height );
unsigned char *LoadImageTGA( string & fileName, int & width, int & height );

#define NO_STRUCT_PADDING	1
#pragma pack( push, NO_STRUCT_PADDING )

struct TGAHeader
{
	unsigned char  stringLength;
	unsigned char  colorMapType;
	unsigned char  imageType;
	unsigned short colorMapIndex;
	unsigned short colorMapLength;
	unsigned char  colorSize;
	unsigned short startingX;
	unsigned short startingY;
	unsigned short width;
	unsigned short height;
	unsigned char  pixelSize;
	unsigned char  attributes;
};

#pragma pack( pop, NO_STRUCT_PADDING )
#undef NO_STRUCT_PADDING

#endif