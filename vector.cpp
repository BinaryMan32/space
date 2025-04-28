#include "vector.h"

#include <iostream.h>
//#include <math.h>

//	General Functions For Angles	///////////////////////////////////////////

inline void NormalizeAngle( float & theAngle )
{
//	while ( theAngle >= 2 * PI ) theAngle -= 2 * PI;
//	while ( theAngle <  0      ) theAngle += 2 * PI;
	_asm
	{
		fldpi
		fadd ST(0), ST(0)	; 2 * PI
		fld theAngle
		fprem1
		fstp theAngle
		ffree ST(0)			; pop stack
		fincstp
	}
}

inline float _declspec(naked) arctan( float yval, float xval )
{
	_asm
	{
		fld yval
		fld xval
		fpatan
		ret
	}
}

inline float _declspec(naked) fsqrt( float num )
{
	_asm
	{
		fld [esp+4]
		fsqrt
		ret
	}
}

inline float Degrees( float theAngle )
{
	return ( theAngle * 180 / PI );
}

inline float Radians( float theAngle )
{
	return ( theAngle * PI / 180 );
}

///////////////////////////////////////////////////////////////////////////////
//	Vector Class	///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Default Constructor
// - Initializes Vector to ( 0 , 0 )
vector2d::vector2d()
{
	x = y = 0;
}

// Angle Constructor
// - Initializes Vector to ( cos( theAngle ) , sin( theAngle ) )
vector2d::vector2d( float theAngle )
{
//	x = float( cos( theAngle ) );
//	y = float( sin( theAngle ) );
	_asm
	{
		mov edi, this
		fld theAngle
		fsincos
		fstp [edi]this.x
		fstp [edi]this.y
	}
}

// Vector Constructor
// - Initializes Vector to ( X , Y )
vector2d::vector2d( float X, float Y )
{
	x = X;
	y = Y;
}

//	Constant arithmetic  //////////////////////////////////////////////////////

inline vector2d operator + ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x + theConstant, theVector.y + theConstant );
}

inline vector2d operator - ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x - theConstant, theVector.y - theConstant );
}

inline vector2d operator * ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x * theConstant, theVector.y * theConstant );
}

inline vector2d operator / ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x / theConstant, theVector.y / theConstant );
}

inline vector2d operator + ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant + theVector.x, theConstant + theVector.y );
}

inline vector2d operator - ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant - theVector.x, theConstant - theVector.y );
}

inline vector2d operator * ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant * theVector.x, theConstant * theVector.y );
}

inline vector2d operator / ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant / theVector.x, theConstant / theVector.y );
}

inline vector2d & operator += ( vector2d & theVector, float theConstant )
{
	theVector.x += theConstant;
	theVector.y += theConstant;
	
	return theVector;
}

inline vector2d & operator -= ( vector2d & theVector, float theConstant )
{
	theVector.x -= theConstant;
	theVector.y -= theConstant;
	
	return theVector;
}

inline vector2d & operator *= ( vector2d & theVector, float theConstant )
{
	theVector.x *= theConstant;
	theVector.y *= theConstant;
	
	return theVector;
}

inline vector2d & operator /= ( vector2d & theVector, float theConstant )
{
	theVector.x /= theConstant;
	theVector.y /= theConstant;
	
	return theVector;
}

//	Vector arithmetic  ////////////////////////////////////////////////////////

inline vector2d operator + ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x + rightVector.x, leftVector.y + rightVector.y );
}

inline vector2d operator - ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x - rightVector.x, leftVector.y - rightVector.y );
}

inline vector2d operator * ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x * rightVector.x, leftVector.y * rightVector.y );
}

inline vector2d operator / ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x / rightVector.x, leftVector.y / rightVector.y );
}

inline vector2d & operator += ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x += rightVector.x;
	leftVector.y += rightVector.y;
	
	return leftVector;
}

inline vector2d & operator -= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x -= rightVector.x;
	leftVector.y -= rightVector.y;
	
	return leftVector;
}

inline vector2d & operator *= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x *= rightVector.x;
	leftVector.y *= rightVector.y;
	
	return leftVector;
}

inline vector2d & operator /= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x /= rightVector.x;
	leftVector.y /= rightVector.y;
	
	return leftVector;
}

inline vector2d operator + ( const vector2d & theVector )
{
	return vector2d( theVector );
}

inline vector2d operator - ( const vector2d & theVector )
{
	return vector2d( -theVector.x, -theVector.y );
}

//	Vector operations  ////////////////////////////////////////////////////////

inline float CrossP( const vector2d & leftVector, const vector2d & rightVector )
{
	return ( leftVector.x * rightVector.y - leftVector.y * rightVector.x );
}

inline float DotP( const vector2d & leftVector, const vector2d & rightVector )
{
	return ( leftVector.x * rightVector.x + leftVector.y * rightVector.y );
}

inline float Sin( const vector2d & leftVector, const vector2d & rightVector )
{
	float leftMag = Mag( leftVector );
	float rightMag = Mag( rightVector );

	if ( leftMag == 0 || rightMag == 0 ) return 0;
	
	return ( CrossP( leftVector, rightVector ) / leftMag / rightMag );
}

inline float Cos( const vector2d & leftVector, const vector2d & rightVector )
{
	float leftMag = Mag( leftVector );
	float rightMag = Mag( rightVector );

	if ( leftMag == 0 || rightMag == 0 ) return 0;
	
	return ( DotP( leftVector, rightVector ) / leftMag / rightMag );
}

inline float Angle( const vector2d & leftVector, const vector2d & rightVector )
{
//	return float( atan2( CrossP( leftVector, rightVector ), DotP( leftVector, rightVector ) ) );
	return arctan( CrossP( leftVector, rightVector ), DotP( leftVector, rightVector ) );
}

inline float Angle( const vector2d & theVector )
{
//	return float( atan2( theVector.y, theVector.x ) );
	return arctan( theVector.y, theVector.x );
}

inline vector2d Rotate( const vector2d & theVector, float theAngle )
{
	vector2d BasisX( theAngle );
	vector2d BasisY( -BasisX.y, BasisX.x );
	
	return ( theVector.x * BasisX + theVector.y * BasisY );
}

inline float Mag( const vector2d & theVector )
{
	return fsqrt( DotP( theVector, theVector ) );
}

inline float MagSquared( const vector2d & theVector )
{
	return DotP( theVector, theVector );
}

inline vector2d Normalize( const vector2d & theVector )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;
		
	return ( theVector * ( 1 / Mag( theVector ) ) );
}

inline vector2d Scale( const vector2d & theVector, float theMagnitude )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;

	return ( theVector * ( theMagnitude / Mag( theVector ) ) );
}

inline vector2d Limit( const vector2d & theVector, float theMagnitude )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;

	float curMagnitude = Mag( theVector );

	if ( curMagnitude <= theMagnitude ) return theVector;

	return ( theVector * ( theMagnitude / curMagnitude ) );
}

inline vector2d Project( const vector2d & leftVector, const vector2d & rightVector )
{
	if ( rightVector.x == 0 && rightVector.y == 0 ) return rightVector;

	return ( DotP( leftVector, rightVector ) / DotP( rightVector, rightVector ) * rightVector );
}

inline vector2d Perpendicular( const vector2d & theVector )
{
	return vector2d( -theVector.y, theVector.x );
}

//	Output	///////////////////////////////////////////////////////////////////

inline ostream & operator << ( ostream & stream, const vector2d & theVector )
{
	stream << "[ " << theVector.x << ' ' << theVector.y << " ]";
	return stream;
}

