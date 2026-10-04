#include <SDL3/SDL_opengl.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

#include "glstream.h"

// Font used for all text, metric compatible with Courier New
#define FONT_FILE		"LiberationMono-Bold.ttf"
#define FONT_PIXELS		48

// Global Variables

glstream glout;

// Static Variables

bool  glstream::GlyphsCreated = false;
bool  glstream::GlyphsFailed = false;
int   glstream::NumStreams = 0;
glstream::Glyph glstream::Glyphs[128];
float glstream::GlyphTop = 0.0f;
float glstream::GlyphBottom = 0.0f;

// Private Functions

bool glstream::CreateGlyphs()
{
	if ( GlyphsCreated ) return true;

	// if OpenGL is not initialized, or the font could not be loaded, do not create glyphs
	if ( GlyphsFailed || SDL_GL_GetCurrentContext() == NULL ) return false;

	GlyphsFailed = true;

	if ( ! TTF_Init() )
	{
		SDL_Log( "TTF_Init() failed: %s", SDL_GetError() );
		return false;
	}

	TTF_Font *Font = TTF_OpenFont( FONT_FILE, FONT_PIXELS );

	if ( Font == NULL )
	{
		SDL_Log( "TTF_OpenFont() failed: %s", SDL_GetError() );
		TTF_Quit();
		return false;
	}

	// glyph images span from the font ascent at the top to the descent at the bottom
	GlyphTop = float( TTF_GetFontAscent( Font ) ) / FONT_PIXELS;

	SDL_Color White = { 255, 255, 255, 255 };

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	for ( int index = 0; index < 128; index++ )
	{
		Glyph & theGlyph = Glyphs[ index ];

		theGlyph.Texture = 0;
		theGlyph.Width = theGlyph.Advance = 0.0f;

		int Advance;
		if ( ! TTF_GetGlyphMetrics( Font, index, NULL, NULL, NULL, NULL, &Advance ) ) continue;
		
		theGlyph.Advance = float( Advance ) / FONT_PIXELS;

		if ( index <= ' ' || index == 127 ) continue;

		SDL_Surface *Rendered = TTF_RenderGlyph_Blended( Font, index, White );
		if ( Rendered == NULL ) continue;

		SDL_Surface *Image = SDL_ConvertSurface( Rendered, SDL_PIXELFORMAT_RGBA32 );
		SDL_DestroySurface( Rendered );
		if ( Image == NULL ) continue;

		theGlyph.Width = float( Image->w ) / FONT_PIXELS;
		GlyphBottom = GlyphTop - float( Image->h ) / FONT_PIXELS;

		glGenTextures( 1, &theGlyph.Texture );
		glBindTexture( GL_TEXTURE_2D, theGlyph.Texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glPixelStorei( GL_UNPACK_ROW_LENGTH, Image->pitch / 4 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Image->w, Image->h,
			0, GL_RGBA, GL_UNSIGNED_BYTE, Image->pixels );

		SDL_DestroySurface( Image );
	}

	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

	TTF_CloseFont( Font );
	TTF_Quit();

	GlyphsFailed = false;
	
	return ( GlyphsCreated = true );
}

void glstream::DestroyGlyphs()
{
	if ( GlyphsCreated )
	{
		// if OpenGL is not initialized, do not destroy textures
		if ( SDL_GL_GetCurrentContext() != NULL )
		{
			for ( int index = 0; index < 128; index++ )
			{
				if ( Glyphs[ index ].Texture != 0 ) glDeleteTextures( 1, &Glyphs[ index ].Texture );
			}
		}

		GlyphsCreated = false;
	}
}

// Sets up the transform and texturing for drawing glyphs at the cursor
void glstream::BeginText()
{
	glPushAttrib( GL_ENABLE_BIT | GL_TEXTURE_BIT );
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glPushMatrix();
	MoveToCursor();
}

// Positions the modelview matrix at the cursor
void glstream::MoveToCursor()
{
	glLoadIdentity();
	glTranslatef( LineX, LineY, 0.0f );
	glScalef( FontSize, FontSize, FontSize );
	glTranslatef( CursorX, 0.0f, 0.0f );
}

void glstream::EndText()
{
	glPopMatrix();
	glPopAttrib();
}

// Constructor / Destructor

glstream::glstream()
{
	LineX = CursorX = 0.0f;
	LineY = 0.0f;
	
	FontSize = 14;

	NumStreams++;
}

glstream::glstream( glstream & other )
{
	CursorX = other.CursorX;
	
	LineX = other.LineX;
	LineY = other.LineY;
	
	FontSize = other.FontSize;

	NumStreams++;
}

glstream & glstream::operator = ( glstream & other )
{
	if ( this != &other )
	{
		CursorX = other.CursorX;
	
		LineX = other.LineX;
		LineY = other.LineY;
	
		FontSize = other.FontSize;
	}

	return *this;
}

glstream::~glstream()
{
	if ( --NumStreams == 0 )
	{
		DestroyGlyphs();
	}
}

// Public Functions

void glstream::MoveTo( float theXposition, float theYposition )
{
	LineX = theXposition;
	LineY = theYposition;
	
	CursorX = 0.0f;
}

void glstream::Move( float theXchange, float theYchange )
{
	LineX += theXchange;
	LineY += theYchange;

	CursorX = 0.0f;
}

void glstream::SetFontSize( float theFontSize )
{
	FontSize = theFontSize;
}

void glstream::NextLine()
{
	LineY -= FontSize;
	
	CursorX = 0.0f;
}

void glstream::PrintChar( char theChar )
{
	if ( theChar < 0 ) return;

	Glyph & theGlyph = Glyphs[ int( theChar ) ];

	if ( theGlyph.Texture != 0 )
	{
		glBindTexture( GL_TEXTURE_2D, theGlyph.Texture );

		glBegin( GL_QUADS );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f,           GlyphBottom );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2f( theGlyph.Width, GlyphBottom );
			glTexCoord2f( 1.0f, 0.0f ); glVertex2f( theGlyph.Width, GlyphTop );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f,           GlyphTop );
		glEnd();
	}

	// advance to the next character position
	glTranslatef( theGlyph.Advance, 0.0f, 0.0f );
	CursorX += theGlyph.Advance;
}

glstream & glstream::operator << ( char theChar )
{
	char theString[ 2 ] = { theChar, '\0' };

	return (*this) << theString;
}

glstream & glstream::operator << ( const char *theString )
{
	if ( CreateGlyphs() )
	{
		BeginText();

		int StringLength = 0;
		while ( theString[ StringLength ] ) StringLength++;
		
		for ( int index = 0; index < StringLength; index++ )
		{
			if ( theString[index] == '\n' )
			{
				NextLine();
				MoveToCursor();
			}
			else
			{
				PrintChar( theString[ index ] );
			}
		} 
	
		EndText();
	}
	
	return *this;
}

glstream & glstream::operator << ( long theNumber )
{
	if ( CreateGlyphs() )
	{
		BeginText();

		if ( theNumber == 0 )
		{
			PrintChar( '0' );
		}
		else
		{
			if ( theNumber < 0 )
			{
				PrintChar( '-' );
				theNumber = -theNumber;
			}

			long divisor = 1;
			long temp = theNumber;
			while ( ( temp /= 10 ) != 0 ) divisor *= 10;

			long digit;
			temp = theNumber;
			while ( divisor != 0 )
			{
				digit = temp / divisor;
				temp -= digit * divisor;
				PrintChar( char( '0' + digit ) );
				divisor /= 10;
			}
		}

		EndText();
	}

	return *this;
}

glstream & glstream::operator << ( int theNumber )
{
	return (*this) << long( theNumber );
}

glstream & glstream::operator << ( double theNumber )
{
	if ( CreateGlyphs() )
	{
		BeginText();

		// print with three decimal places
		char DecimalString[ 64 ];
		snprintf( DecimalString, sizeof( DecimalString ), "%.3f", theNumber );

		for ( char *CharPtr = DecimalString; *CharPtr; CharPtr++ ) PrintChar( *CharPtr );

		EndText();
	}

	return *this;
}

glstream & glstream::operator << ( float theNumber )
{
	return (*this) << double( theNumber );
}
