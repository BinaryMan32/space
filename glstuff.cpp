#include "glstuff.h"

RGBColor MakeRGBColor( unsigned char theRed, unsigned char theGreen, unsigned char theBlue, unsigned char theAlpha )
{
	RGBColor temp;
	temp.red = theRed;
	temp.green = theGreen;
	temp.blue = theBlue;
	temp.alpha = theAlpha;
	return temp;
}

RGBColor ColorBlack =	{	0,		0,		0,		255		};
RGBColor ColorGray =	{	128,	128,	128,	255		};
RGBColor ColorWhite =	{	255,	255,	255,	255		};

RGBColor ColorRed =		{	255,	0,		0,		255		};
RGBColor ColorGreen =	{	0,		255,	0,		255		};
RGBColor ColorBlue =	{	0,		0,		255,	255		};

RGBColor ColorYellow =	{	255,	255,	0,		255		};
RGBColor ColorPurple =	{	255,	0,		255,	255		};
RGBColor ColorAqua =	{	0,		255,	255,	255		};

glClass gl;

int glClass::GetWidth()
{
	return width;
}

int glClass::GetHeight()
{
	return height;
}

void glClass::Resize( int theWidth, int theHeight )
{
	width = theWidth;
	height = theHeight;

	glViewport( 0, 0, width, height );
}

bool glClass::Init()
{
	Context = SDL_GL_CreateContext( Program.GetWindow() );

	if ( Context == NULL )
	{
		SDL_Log( "SDL_GL_CreateContext() failed: %s", SDL_GetError() );
		return false;
	}

	// synchronize buffer swaps with the display refresh when supported
	SDL_GL_SetSwapInterval( 1 );

	glDrawBuffer( GL_BACK );
	
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );

	// texture mapping settings
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	int WindowWidth, WindowHeight;
	SDL_GetWindowSizeInPixels( Program.GetWindow(), &WindowWidth, &WindowHeight );
	
	// set projection
	Resize( WindowWidth, WindowHeight );

	return true;
}

void glClass::Destroy()
{
	if ( Context != NULL )
	{
		SDL_GL_DestroyContext( Context );
		Context = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  opengl texture functions  //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

glClass::Texture *glClass::TextureLoad( const std::string & fileName )
{
	List<Texture *>::Iterator TextureIterator = TextureList.Start();

	while ( ++TextureIterator )
	{
		if ( TextureIterator->fileName == fileName )
		{
			TextureIterator->referenceCount += 1;
			return *TextureIterator;
		}
	}

	Texture *theTexture = new Texture;
	
	if ( theTexture != NULL )
	{
		unsigned char *data = LoadImage( fileName, theTexture->width, theTexture->height );

		if ( data != NULL )
		{
			if ( theTexture->width > 0 && theTexture->height > 0 && ( theTexture->height >= theTexture->width ) )
			{
				theTexture->numFrames = theTexture->height / theTexture->width;
				theTexture->height /= theTexture->numFrames;
			
				theTexture->tag = new GLuint [ theTexture->numFrames ];

				if ( theTexture->tag != NULL )
				{
					theTexture->fileName = fileName;
					theTexture->referenceCount = 1;

					int FrameOffset = theTexture->width * theTexture->height * 4;

					glGenTextures( theTexture->numFrames, theTexture->tag );

					for ( int index = 0; index < theTexture->numFrames; index++ )
					{
						glBindTexture( GL_TEXTURE_2D, theTexture->tag[ index ] );

						glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
						glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

						glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
						glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

						glTexImage2D( GL_TEXTURE_2D, 0, 4, theTexture->width, theTexture->height,
							0, GL_BGRA, GL_UNSIGNED_BYTE, data + FrameOffset * index  );
					}

					TextureList.Insert( theTexture );
				}
				else
				{
					delete theTexture;
					theTexture = NULL;
				}
			}			
			else
			{
				delete theTexture;
				theTexture = NULL;
			}
		
			delete [] data;
		}
		else
		{
			delete theTexture;
			theTexture = NULL;
		}
	}

	return theTexture;
}

void glClass::TextureAddReference( Texture *theTexture )
{
	if ( theTexture == NULL ) return;

	theTexture->referenceCount += 1;
}

void glClass::TextureRelease( Texture *theTexture )
{
	if ( theTexture == NULL ) return;
	if ( theTexture->referenceCount -= 1 ) return;

	glDeleteTextures( theTexture->numFrames, theTexture->tag );
	delete [] theTexture->tag;

	List<Texture *>::Iterator TextureIterator = TextureList.Start();

	while ( ++TextureIterator )
	{
		if ( *TextureIterator == theTexture )
		{
			delete *TextureIterator;
			TextureList.Remove( TextureIterator );
			return;
		}
	}
}
	
void glClass::TextureSelect( Texture *theTexture, int theFrame )
{
	if ( theTexture == NULL ) return;

	if ( theFrame < 0 ) theFrame = 0;
	if ( theFrame >= theTexture->numFrames ) theFrame = theTexture->numFrames - 1;

	glBindTexture( GL_TEXTURE_2D, theTexture->tag[ theFrame ] );
}
