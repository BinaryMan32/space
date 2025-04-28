// glstream.h: interface for the glstream class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GLSTREAM_H
#define GLSTREAM_H

class glstream  
{
private:
	
	static bool DisplayListsCreated;
	static int  DisplayListBase;
	static int  NumStreams;
	static GLYPHMETRICSFLOAT agmf[256];

	float LineX, LineY, CursorX;
	float FontSize;

	bool CreateDisplayLists();
	void DestroyDisplayLists();

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
	glstream & operator << ( char *theString );
	
	glstream & operator << ( long theNumber );
	glstream & operator << ( int theNumber );
	
	glstream & operator << ( double theNumber );
	glstream & operator << ( float theNumber );
};

extern glstream glout;

#endif
