#ifndef GLSTUFF_H
#define GLSTUFF_H

#include <SDL3/SDL_opengl.h>
#include <GL/glu.h>

#include "App.h"
#include "gamestring.h"
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

	SDL_GLContext Context;

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
		Context = NULL;
	}
		
	int GetWidth();
	int GetHeight();
	
	void Resize( int theWidth, int theHeight );
	bool Init();
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