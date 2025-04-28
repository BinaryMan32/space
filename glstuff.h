#ifndef GLSTUFF_H
#define GLSTUFF_H

#include <windows.h>
#include <windowsx.h>

#include <wingdi.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "WinApp.h"
#include "string.h"
#include "image.h"
#include "List.h"

struct RGBColor
{
	unsigned char red,green,blue,alpha;
};

RGBColor MakeRGBColor( unsigned char theRed, unsigned char theGreen, unsigned char theBlue, unsigned char theAlpha = 255 );

extern RGBColor ColorBlack,		ColorGray,		ColorWhite,
				ColorRed,		ColorGreen,		ColorBlue,
				ColorYellow,	ColorPurple,	ColorAqua;

class glTexture;
class glClass;

extern glClass gl;

class glClass
{
	private:
	
	struct Texture
	{
		string fileName;
		int width, height;
		int referenceCount;
		int numFrames;
		GLuint *tag;
	};
	
	List<Texture *> TextureList;

	int width;
	int height;

	friend class glTexture;

	Texture *TextureLoad( string & fileName );
	void TextureAddReference( Texture *theTexture );
	void TextureRelease( Texture *theTexture );
	void TextureSelect( Texture *theTexture, int theFrame );

	public:
	
	glClass()
	{
		width = height = 0;
	}
		
	int GetWidth();
	int GetHeight();
	
	void Resize( int theWidth, int theHeight );
	void Init( int theWidth, int theHeight );
	void Init();
	void Destroy();
};

class glTexture
{
	private:
	
	glClass::Texture *TexturePtr;

	public:
	
	glTexture()
	{
		TexturePtr = NULL;
	}

	glTexture( string fileName )
	{
		TexturePtr = gl.TextureLoad( fileName );
	}

	glTexture( glTexture & theTexture )
	{
		TexturePtr = theTexture.TexturePtr;
		gl.TextureAddReference( TexturePtr );
	}

	glTexture operator = ( glTexture theTexture )
	{
		gl.TextureRelease( TexturePtr );
		TexturePtr = theTexture.TexturePtr;
		gl.TextureAddReference( TexturePtr );
		return *this;
	}
	
	~glTexture()
	{
		gl.TextureRelease( TexturePtr );
		TexturePtr = NULL;
	}

	operator bool()
	{
		 return ( TexturePtr != NULL );
	}

	bool Valid()
	{
		 return ( TexturePtr != NULL );
	}

	void Load( string fileName )
	{
		gl.TextureRelease( TexturePtr );
		TexturePtr = gl.TextureLoad( fileName );
	}

	void Select( int theFrame )
	{
		gl.TextureSelect( TexturePtr, theFrame );
	}

	int GetWidth()
	{
		if ( TexturePtr == NULL ) return 0;
		return TexturePtr->width;
	}

	int GetHeight()
	{
		if ( TexturePtr == NULL ) return 0;
		return TexturePtr->height;
	}

	int GetNumFrames()
	{
		if ( TexturePtr == NULL ) return 0;
		return TexturePtr->numFrames;
	}
};

#endif