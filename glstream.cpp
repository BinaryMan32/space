#include <windows.h>
#include <wingdi.h>
#include <gl/gl.h>
#include <stdlib.h>

#include "glstream.h"

// Global Variables

glstream glout;

// Static Variables

bool glstream::DisplayListsCreated = false;
int  glstream::DisplayListBase = 1024;
int  glstream::NumStreams = 0;
GLYPHMETRICSFLOAT glstream::agmf[256];

// Private Functions

bool glstream::CreateDisplayLists()
{
	if ( DisplayListsCreated ) return true;

	HDC DeviceContext = wglGetCurrentDC();
	
	// if OpenGL is not initialized, do not create lists
	if ( DeviceContext == NULL ) return false;
	
	LOGFONT     lf;
	HFONT       hFont, hOldFont;

	// create a TrueType font
	ZeroMemory( &lf, sizeof( LOGFONT ) );
	lf.lfHeight				= -20;
	lf.lfWeight				= FW_BOLD;
	lf.lfCharSet			= ANSI_CHARSET;
	lf.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	lf.lfQuality			= DEFAULT_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE | DEFAULT_PITCH;
	lf.lfFaceName[0]		= 'C';
	lf.lfFaceName[1]		= 'o';
	lf.lfFaceName[2]		= 'u';
	lf.lfFaceName[3]		= 'r';
	lf.lfFaceName[4]		= 'i';
	lf.lfFaceName[5]		= 'e';
	lf.lfFaceName[6]		= 'r';
	lf.lfFaceName[7]		= ' ';
	lf.lfFaceName[8]		= 'N';
	lf.lfFaceName[9]		= 'e';
	lf.lfFaceName[10]		= 'w';
	lf.lfFaceName[11]		= '\0';

	hFont = CreateFontIndirect( &lf );
	hOldFont = (HFONT)SelectObject( DeviceContext, hFont );

	wglUseFontOutlines( DeviceContext, 0, 255, DisplayListBase, 0.1f, 0.0f, WGL_FONT_POLYGONS, agmf );

	DeleteObject( SelectObject( DeviceContext, hOldFont ) );
   
	return ( DisplayListsCreated = true );
}

void glstream::DestroyDisplayLists()
{
	if ( DisplayListsCreated )
	{
		// if OpenGL is not initialized, do not destroy lists
		if ( wglGetCurrentDC() != NULL ) glDeleteLists( DisplayListBase, 255 );

		DisplayListsCreated = false;
	}
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
		DestroyDisplayLists();
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
	glCallList( DisplayListBase + theChar );
	CursorX += agmf[ theChar ].gmfCellIncX;
}

glstream & glstream::operator << ( char theChar )
{
	if ( CreateDisplayLists() )
	{
		if ( theChar == '\n' )
		{
			NextLine();
		}
		else
		{
			PrintChar( theChar );
		}
	}

	return *this;
}

glstream & glstream::operator << ( char *theString )
{
	if ( CreateDisplayLists() )
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef( LineX, LineY, 0.0f );
		glScalef( FontSize, FontSize, FontSize );
		glTranslatef( CursorX, 0.0f, 0.0f );

		int StringLength = 0;
		while ( theString[ StringLength ] ) StringLength++;
		
		for ( int index = 0; index < StringLength; index++ )
		{
			if ( theString[index] == '\n' )
			{
				NextLine();
				glLoadIdentity();
				glTranslatef( LineX, LineY, 0.0f );
				glScalef( FontSize, FontSize, FontSize );
				glTranslatef( CursorX, 0.0f, 0.0f );
			}
			else
			{
				PrintChar( theString[ index ] );
			}
		} 
	
		glPopMatrix();
	}
	
	return *this;
}

glstream & glstream::operator << ( long theNumber )
{
	if ( CreateDisplayLists() )
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef( LineX, LineY, 0.0f );
		glScalef( FontSize, FontSize, FontSize );
		glTranslatef( CursorX, 0.0f, 0.0f );

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

		glPopMatrix();
	}

	return *this;
}

glstream & glstream::operator << ( int theNumber )
{
	return (*this) << long( theNumber );
}

glstream & glstream::operator << ( double theNumber )
{
	if ( CreateDisplayLists() )
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef( LineX, LineY, 0.0f );
		glScalef( FontSize, FontSize, FontSize );
		glTranslatef( CursorX, 0.0f, 0.0f );

		int Negative;
		int DecimalPoint;
		char *DecimalString = _fcvt( theNumber, 3, &DecimalPoint, &Negative );

		if ( Negative ) PrintChar( '-' );
		
		if ( DecimalPoint <= 0 )
		{
			PrintChar( '0' );
			PrintChar( '.' );
			while ( DecimalPoint++ < 0 ) PrintChar( '0' );
		}
		else
		{
			while ( DecimalPoint-- > 0 ) PrintChar( *DecimalString++ );
			PrintChar( '.' );
		}

		while ( *DecimalString ) PrintChar( *DecimalString++ );

		glPopMatrix();
	}

	return *this;
}

glstream & glstream::operator << ( float theNumber )
{
	return (*this) << double( theNumber );
}
