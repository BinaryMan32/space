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

void glClass::Init( int theWidth, int theHeight )
{
	PIXELFORMATDESCRIPTOR pfd =
	{ 
	    sizeof( PIXELFORMATDESCRIPTOR ),
	    1,                        // version number 
	    PFD_DRAW_TO_WINDOW |      // support window 
	    PFD_SUPPORT_OPENGL |      // support OpenGL 
	    PFD_GENERIC_ACCELERATED | // hardware acceleration
		PFD_DOUBLEBUFFER,         // double buffered 
	    PFD_TYPE_RGBA,            // RGBA type 
	    24,                       // 24-bit color depth 
		0, 0, 0, 0, 0, 0,         // color bits ignored 
	    8,                        // 8-bit alpha buffer 
	    0,                        // shift bit ignored 
	    0,                        // no accumulation buffer 
	    0, 0, 0, 0,               // accum bits ignored 
	    32,	                      // 32-bit z-buffer     
	    0,                        // no stencil buffer 
	    0,                        // no auxiliary buffer 
	    PFD_MAIN_PLANE,           // main layer
	    0,                        // reserved 
	    0, 0, 0                   // layer masks ignored 
	}; 
 
	int FormatIndex = ChoosePixelFormat( Program.GetHDC(), &pfd );

	DescribePixelFormat( Program.GetHDC(), FormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd );

	SetPixelFormat( Program.GetHDC(), FormatIndex, &pfd );
		
	HGLRC hglrc = wglCreateContext( Program.GetHDC() );
	
	wglMakeCurrent( Program.GetHDC(), hglrc );
	
	glDrawBuffer( GL_BACK );
	
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );

	// texture mapping settings
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	// set projection
	Resize( theWidth, theHeight );
}

void glClass::Init()
{
	PIXELFORMATDESCRIPTOR pfd =
	{ 
	    sizeof( PIXELFORMATDESCRIPTOR ),
	    1,                        // version number 
	    PFD_DRAW_TO_WINDOW |      // support window 
	    PFD_SUPPORT_OPENGL |      // support OpenGL 
	    PFD_GENERIC_ACCELERATED | // hardware acceleration
		PFD_DOUBLEBUFFER,         // double buffered 
	    PFD_TYPE_RGBA,            // RGBA type 
	    24,                       // 24-bit color depth 
		0, 0, 0, 0, 0, 0,         // color bits ignored 
	    8,                        // 8-bit alpha buffer 
	    0,                        // shift bit ignored 
	    0,                        // no accumulation buffer 
	    0, 0, 0, 0,               // accum bits ignored 
	    32,	                      // 32-bit z-buffer     
	    0,                        // no stencil buffer 
	    0,                        // no auxiliary buffer 
	    PFD_MAIN_PLANE,           // main layer
	    0,                        // reserved 
	    0, 0, 0                   // layer masks ignored 
	}; 
 
	int FormatIndex = ChoosePixelFormat( Program.GetHDC(), &pfd );

	DescribePixelFormat( Program.GetHDC(), FormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd );

	SetPixelFormat( Program.GetHDC(), FormatIndex, &pfd );
		
	HGLRC hglrc = wglCreateContext( Program.GetHDC() );
	
	wglMakeCurrent( Program.GetHDC(), hglrc );
	
	glDrawBuffer( GL_BACK );
	
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );

	// texture mapping settings
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	RECT WindowRect;
	GetClientRect( Program.GetHWND(), &WindowRect );
	
	// set projection
	Resize( WindowRect.right, WindowRect.bottom );
}

void glClass::Destroy()
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( wglGetCurrentContext() );
}

////////////////////////////////////////////////////////////////////////////////
//  opengl texture functions  //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

glClass::Texture *glClass::TextureLoad( string & fileName )
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
							0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data + FrameOffset * index  );
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
