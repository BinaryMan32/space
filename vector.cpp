#include "vector.h"

#include <iostream>
#include <math.h>

//	General Functions For Angles	///////////////////////////////////////////

void NormalizeAngle( float & theAngle )
{
//	while ( theAngle >= 2 * PI ) theAngle -= 2 * PI;
//	while ( theAngle <  0      ) theAngle += 2 * PI;
	theAngle = remainderf( theAngle, 2 * PI );
}

float arctan( float yval, float xval )
{
	return atan2f( yval, xval );
}

float fsqrt( float num )
{
	return sqrtf( num );
}

float Degrees( float theAngle )
{
	return ( theAngle * 180 / PI );
}

float Radians( float theAngle )
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
	x = cosf( theAngle );
	y = sinf( theAngle );
}

// Vector Constructor
// - Initializes Vector to ( X , Y )
vector2d::vector2d( float X, float Y )
{
	x = X;
	y = Y;
}

//	Constant arithmetic  //////////////////////////////////////////////////////

vector2d operator + ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x + theConstant, theVector.y + theConstant );
}

vector2d operator - ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x - theConstant, theVector.y - theConstant );
}

vector2d operator * ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x * theConstant, theVector.y * theConstant );
}

vector2d operator / ( const vector2d & theVector, float theConstant )
{
	return vector2d( theVector.x / theConstant, theVector.y / theConstant );
}

vector2d operator + ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant + theVector.x, theConstant + theVector.y );
}

vector2d operator - ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant - theVector.x, theConstant - theVector.y );
}

vector2d operator * ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant * theVector.x, theConstant * theVector.y );
}

vector2d operator / ( float theConstant, const vector2d & theVector )
{
	return vector2d( theConstant / theVector.x, theConstant / theVector.y );
}

vector2d & operator += ( vector2d & theVector, float theConstant )
{
	theVector.x += theConstant;
	theVector.y += theConstant;
	
	return theVector;
}

vector2d & operator -= ( vector2d & theVector, float theConstant )
{
	theVector.x -= theConstant;
	theVector.y -= theConstant;
	
	return theVector;
}

vector2d & operator *= ( vector2d & theVector, float theConstant )
{
	theVector.x *= theConstant;
	theVector.y *= theConstant;
	
	return theVector;
}

vector2d & operator /= ( vector2d & theVector, float theConstant )
{
	theVector.x /= theConstant;
	theVector.y /= theConstant;
	
	return theVector;
}

//	Vector arithmetic  ////////////////////////////////////////////////////////

vector2d operator + ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x + rightVector.x, leftVector.y + rightVector.y );
}

vector2d operator - ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x - rightVector.x, leftVector.y - rightVector.y );
}

vector2d operator * ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x * rightVector.x, leftVector.y * rightVector.y );
}

vector2d operator / ( const vector2d & leftVector, const vector2d & rightVector )
{
	return vector2d( leftVector.x / rightVector.x, leftVector.y / rightVector.y );
}

vector2d & operator += ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x += rightVector.x;
	leftVector.y += rightVector.y;
	
	return leftVector;
}

vector2d & operator -= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x -= rightVector.x;
	leftVector.y -= rightVector.y;
	
	return leftVector;
}

vector2d & operator *= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x *= rightVector.x;
	leftVector.y *= rightVector.y;
	
	return leftVector;
}

vector2d & operator /= ( vector2d & leftVector, const vector2d & rightVector )
{
	leftVector.x /= rightVector.x;
	leftVector.y /= rightVector.y;
	
	return leftVector;
}

vector2d operator + ( const vector2d & theVector )
{
	return vector2d( theVector );
}

vector2d operator - ( const vector2d & theVector )
{
	return vector2d( -theVector.x, -theVector.y );
}

//	Vector operations  ////////////////////////////////////////////////////////

float CrossP( const vector2d & leftVector, const vector2d & rightVector )
{
	return ( leftVector.x * rightVector.y - leftVector.y * rightVector.x );
}

float DotP( const vector2d & leftVector, const vector2d & rightVector )
{
	return ( leftVector.x * rightVector.x + leftVector.y * rightVector.y );
}

float Sin( const vector2d & leftVector, const vector2d & rightVector )
{
	float leftMag = Mag( leftVector );
	float rightMag = Mag( rightVector );

	if ( leftMag == 0 || rightMag == 0 ) return 0;
	
	return ( CrossP( leftVector, rightVector ) / leftMag / rightMag );
}

float Cos( const vector2d & leftVector, const vector2d & rightVector )
{
	float leftMag = Mag( leftVector );
	float rightMag = Mag( rightVector );

	if ( leftMag == 0 || rightMag == 0 ) return 0;
	
	return ( DotP( leftVector, rightVector ) / leftMag / rightMag );
}

float Angle( const vector2d & leftVector, const vector2d & rightVector )
{
//	return float( atan2( CrossP( leftVector, rightVector ), DotP( leftVector, rightVector ) ) );
	return arctan( CrossP( leftVector, rightVector ), DotP( leftVector, rightVector ) );
}

float Angle( const vector2d & theVector )
{
//	return float( atan2( theVector.y, theVector.x ) );
	return arctan( theVector.y, theVector.x );
}

vector2d Rotate( const vector2d & theVector, float theAngle )
{
	vector2d BasisX( theAngle );
	vector2d BasisY( -BasisX.y, BasisX.x );
	
	return ( theVector.x * BasisX + theVector.y * BasisY );
}

float Mag( const vector2d & theVector )
{
	return fsqrt( DotP( theVector, theVector ) );
}

float MagSquared( const vector2d & theVector )
{
	return DotP( theVector, theVector );
}

vector2d Normalize( const vector2d & theVector )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;
		
	return ( theVector * ( 1 / Mag( theVector ) ) );
}

vector2d Scale( const vector2d & theVector, float theMagnitude )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;

	return ( theVector * ( theMagnitude / Mag( theVector ) ) );
}

vector2d Limit( const vector2d & theVector, float theMagnitude )
{
	if ( theVector.x == 0 && theVector.y == 0 ) return theVector;

	float curMagnitude = Mag( theVector );

	if ( curMagnitude <= theMagnitude ) return theVector;

	return ( theVector * ( theMagnitude / curMagnitude ) );
}

vector2d Project( const vector2d & leftVector, const vector2d & rightVector )
{
	if ( rightVector.x == 0 && rightVector.y == 0 ) return rightVector;

	return ( DotP( leftVector, rightVector ) / DotP( rightVector, rightVector ) * rightVector );
}

vector2d Perpendicular( const vector2d & theVector )
{
	return vector2d( -theVector.y, theVector.x );
}

//	Output	///////////////////////////////////////////////////////////////////

std::ostream & operator << ( std::ostream & stream, const vector2d & theVector )
{
	stream << "[ " << theVector.x << ' ' << theVector.y << " ]";
	return stream;
}

