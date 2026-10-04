#include <stdio.h>
#include "gamestring.h"

#include "image.h"

unsigned char *LoadImage( string & fileName, int & width, int & height )
{
	int dotPosition = fileName.Find( '.' ) + 1;
	if ( dotPosition == 0 ) return NULL;
		
	string fileExtension( fileName.SubString( dotPosition, fileName.Length() - dotPosition ) );
	fileExtension.MakeLowerCase();

	if ( fileExtension == "tga" )
	{
		return LoadImageTGA( fileName, width, height );
	}

	return NULL;
}

unsigned char *LoadImageTGA( string & fileName, int & width, int & height )
{
	TGAHeader imageInfo;

	FILE *filePtr = fopen( fileName, "rb" );

	fread( &imageInfo, sizeof( imageInfo ), 1, filePtr );
	fseek( filePtr, imageInfo.stringLength, SEEK_CUR );

	width = imageInfo.width;
	height = imageInfo.height;

	if ( imageInfo.pixelSize != 32 ) return NULL;
	
	unsigned char *data = new unsigned char [ width * height * 4 ];
	if ( data == NULL ) return NULL;

	fread( data, width * 4, height, filePtr );

	return data;
}
