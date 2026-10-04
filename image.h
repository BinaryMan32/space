#ifndef IMAGE_H
#define IMAGE_H

#include <string>

unsigned char *LoadImage( const std::string & fileName, int & width, int & height );
unsigned char *LoadImageTGA( const std::string & fileName, int & width, int & height );

#pragma pack( push, 1 )

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

static_assert( sizeof( TGAHeader ) == 18, "TGA header must not be padded" );

#pragma pack( pop )

#endif