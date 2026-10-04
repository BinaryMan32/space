#ifndef VECTOR_H
#define VECTOR_H

#include <iosfwd>

#define PI 3.14159265358979f

extern void NormalizeAngle( float & theAngle );
extern float arctan( float dy, float dx );
extern float fsqrt( float num );
extern float Degrees( float theAngle );
extern float Radians( float theAngle );

class vector2d
{
	public:
	
	float x, y;

	vector2d();
	vector2d( float theAngle );
	vector2d( float X, float Y );
};

extern vector2d   operator +  ( float theConstant, const vector2d & theVector );
extern vector2d   operator -  ( float theConstant, const vector2d & theVector );
extern vector2d   operator *  ( float theConstant, const vector2d & theVector );
extern vector2d   operator /  ( float theConstant, const vector2d & theVector );

extern vector2d   operator +  ( const vector2d & theVector, float theConstant );
extern vector2d   operator -  ( const vector2d & theVector, float theConstant );
extern vector2d   operator *  ( const vector2d & theVector, float theConstant );
extern vector2d   operator /  ( const vector2d & theVector, float theConstant );

extern vector2d & operator += ( vector2d & theVector, float theConstant );
extern vector2d & operator -= ( vector2d & theVector, float theConstant );
extern vector2d & operator *= ( vector2d & theVector, float theConstant );
extern vector2d & operator /= ( vector2d & theVector, float theConstant );

extern vector2d   operator +  ( const vector2d & leftVector, const vector2d & rightVector );
extern vector2d   operator -  ( const vector2d & leftVector, const vector2d & rightVector );
extern vector2d   operator *  ( const vector2d & leftVector, const vector2d & rightVector );
extern vector2d   operator /  ( const vector2d & leftVector, const vector2d & rightVector );

extern vector2d & operator += ( vector2d & leftVector, const vector2d & rightVector );
extern vector2d & operator -= ( vector2d & leftVector, const vector2d & rightVector );
extern vector2d & operator *= ( vector2d & leftVector, const vector2d & rightVector );
extern vector2d & operator /= ( vector2d & leftVector, const vector2d & rightVector );

extern vector2d   operator +  ( const vector2d & theVector );
extern vector2d   operator -  ( const vector2d & theVector );
	
extern float CrossP( const vector2d & leftVector, const vector2d & rightVector );
extern float DotP( const vector2d & leftVector, const vector2d & rightVector );
	
extern float Sin( const vector2d & leftVector, const vector2d & rightVector );
extern float Cos( const vector2d & leftVector, const vector2d & rightVector );
	
extern float Angle( const vector2d & theVector );
extern float Angle( const vector2d & leftVector, const vector2d & rightVector );

extern vector2d Rotate( const vector2d & theVector, float theAngle );

extern float Mag( const vector2d & theVector );
extern float MagSquared( const vector2d & theVector );

extern vector2d Normalize( const vector2d & theVector );
extern vector2d Scale( const vector2d & theVector, float theMagnitude );
extern vector2d Limit( const vector2d & theVector, float theMagnitude );

extern vector2d Project( const vector2d & leftVector, const vector2d & rightVector );
extern vector2d Perpendicular( const vector2d & theVector );

extern std::ostream & operator << ( std::ostream & stream, const vector2d & theVector );

#endif