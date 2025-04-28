#include "StarField.h"

// Layer Class Functions	///////////////////////////////////////////////////

StarField::Layer::Layer()
{
	Radius = 0;
	NumStars = 0;

	Stars = NULL;

	VelocityModifier = 1;
	Color = ColorWhite;
}

StarField::Layer::~Layer()
{
	if ( Stars != NULL ) delete [] Stars;
}

void StarField::Layer::Init( int theNumStars, float theRadius, float theVelocityModifier, RGBColor theColor )
{
	if ( Stars != NULL ) delete [] Stars;

	NumStars = theNumStars;
	Radius = theRadius;
	VelocityModifier = theVelocityModifier;
	Color = theColor;

	Stars = new float [ NumStars * 2 ];
	float *StarPtr = Stars;

	for ( int index = NumStars * 2; index > 0; index--, StarPtr++ )
		*StarPtr = float( rand()%int(Radius * 2) - Radius );
}

void StarField::Layer::Move( float dx, float dy )
{
	dx *= VelocityModifier;
	dy *= VelocityModifier;

	float *xPtr = &Stars[0];
	float *yPtr = &Stars[1];
	
	for ( int index = NumStars; index > 0; index--, xPtr+=2, yPtr+=2 )
	{
		*xPtr += dx;
		*yPtr += dy;
		
		if ( *xPtr < -Radius ) *xPtr += Radius * 2;
		if ( *xPtr >  Radius ) *xPtr -= Radius * 2;

		if ( *yPtr < -Radius ) *yPtr += Radius * 2;
		if ( *yPtr >  Radius ) *yPtr -= Radius * 2;
	}

}

void StarField::Layer::Draw()
{
	glColor3ub( Color.red, Color.green, Color.blue );
	glVertexPointer( 2, GL_FLOAT, 0, Stars );
	glDrawArrays( GL_POINTS, 0, NumStars );
}

//	StarField Class	Functions	///////////////////////////////////////////////

void StarField::Init( float Radius )
{
	OldX = OldY = 0.0f;
	
	Layers[ 0 ].Init(  80, Radius, 0.6f, MakeRGBColor( 255, 255, 255 ) );
	Layers[ 1 ].Init( 160, Radius, 0.5f, MakeRGBColor( 224, 224, 224 ) );
	Layers[ 2 ].Init( 240, Radius, 0.4f, MakeRGBColor( 192, 192, 192 ) );
	Layers[ 3 ].Init( 320, Radius, 0.3f, MakeRGBColor( 160, 160, 160 ) );
	Layers[ 4 ].Init( 400, Radius, 0.2f, MakeRGBColor( 128, 128, 128 ) );
	Layers[ 5 ].Init( 480, Radius, 0.1f, MakeRGBColor(  96,  96,  96 ) );
}

void StarField::Move( float x, float y )
{
	for ( int index = 0; index < 6; index++ )
		Layers[ index ].Move( OldX - x, OldY - y );
	
	OldX = x;
	OldY = y;
}

void StarField::Draw()
{
	glEnableClientState( GL_VERTEX_ARRAY );

	for ( int index = 0; index < 6; index++ )
	{
		glPointSize( ( 6 - index ) / 3.0f );
		Layers[ index ].Draw();
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
}
