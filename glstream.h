// glstream.h: interface for the glstream class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GLSTREAM_H
#define GLSTREAM_H

class glstream  
{
private:
	
	// one texture per printable ASCII character, sized in em units
	struct Glyph
	{
		unsigned int Texture;
		float Width, Advance;
	};

	static bool GlyphsCreated;
	static bool GlyphsFailed;
	static int  NumStreams;
	static Glyph Glyphs[128];
	static float GlyphTop, GlyphBottom;

	float LineX, LineY, CursorX;
	float FontSize;

	bool CreateGlyphs();
	void DestroyGlyphs();

	void BeginText();
	void MoveToCursor();
	void EndText();

public:
	
	glstream();
	glstream( glstream & other );
	glstream & operator = ( glstream & other );
	~glstream();
	
	void MoveTo( float theXposition, float theYposition );
	void Move( float theXchange, float theYchange );

	void SetFontSize( float theFontSize );

	void NextLine();

	void PrintChar( char theChar );

	glstream & operator << ( char theChar );
	glstream & operator << ( const char *theString );
	
	glstream & operator << ( long theNumber );
	glstream & operator << ( int theNumber );
	
	glstream & operator << ( double theNumber );
	glstream & operator << ( float theNumber );
};

extern glstream glout;

#endif
