#include <ctype.h>
#include <stdio.h>

#include "image.h"

unsigned char *LoadImage( const std::string & fileName, int & width, int & height )
{
	std::string::size_type dotPosition = fileName.rfind( '.' );
	if ( dotPosition == std::string::npos ) return NULL;
		
	std::string fileExtension( fileName.substr( dotPosition + 1 ) );
	for ( char & c : fileExtension ) c = char( tolower( (unsigned char) c ) );

	if ( fileExtension == "tga" )
	{
		return LoadImageTGA( fileName, width, height );
	}

	return NULL;
}

unsigned char *LoadImageTGA( const std::string & fileName, int & width, int & height )
{
	TGAHeader imageInfo;

	FILE *filePtr = fopen( fileName.c_str(), "rb" );
	if ( filePtr == NULL ) return NULL;

	fread( &imageInfo, sizeof( imageInfo ), 1, filePtr );
	fseek( filePtr, imageInfo.stringLength, SEEK_CUR );

	width = imageInfo.width;
	height = imageInfo.height;

	if ( imageInfo.pixelSize != 32 )
	{
		fclose( filePtr );
		return NULL;
	}
	
	unsigned char *data = new unsigned char [ width * height * 4 ];

	fread( data, width * 4, height, filePtr );
	fclose( filePtr );

	return data;
}
