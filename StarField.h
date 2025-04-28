#ifndef STARFIELD_H
#define STARFIELD_H

#include "glstuff.h"

class StarField
{
	class Layer
	{
		float Radius;
		int NumStars;
		
		float *Stars;

		float VelocityModifier;
		RGBColor Color;

	public:

		Layer();
		~Layer();
		void Init( int theNumStars, float theRadius, float theVelocityModifier, RGBColor theColor );
		void Move( float dx, float dy );
		void Draw();

	} Layers[ 6 ];

	float OldX, OldY;

public:

	void Init( float Radius );
	void Move( float x, float y );
	void Draw();

};

#endif