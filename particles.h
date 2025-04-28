#ifndef PARTICLES_H
#define PARTICLES_H

#include "glstuff.h"
#include "list.h"
#include "vector.h"

extern float frand();

class ParticleSystemClass
{
	class ParticleGroup
	{
		RGBColor StartColor;	// initial group color
		RGBColor EndColor;		// final group color
		
		int NumParticles;
		float Radius;

		vector2d *Position;
		vector2d *Velocity;

		float Lifetime;			// lifetime of group in seconds
		float Counter;			// time since creation in seconds

	public:

		ParticleGroup( int theNumParticles );
		~ParticleGroup();

		void SetLifetime( float theLifetime );
		void SetColor( RGBColor theStartColor, RGBColor theEndColor );
		void SetRadius( float theRadius );

		void SetPosition( const vector2d & thePosition );
		void AddPositionOffset( const vector2d & theOffset );
		void AddRandomPosition( float min, float max );

		void SetVelocity( const vector2d & theVelocity );
		void AddVelocityOffset( const vector2d & theOffset );
		void AddRandomVelocity( float min, float max );

		void Draw();
		bool Move();
	};

	List<ParticleGroup *> ParticleGroupList;

	glTexture *ParticleTexture;

	ParticleGroup *GroupPtr;

public:

	ParticleSystemClass();
	bool Init();

	void Begin( int NumParticles );

	void SetLifetime( float theLifetime );
	void SetColor( RGBColor theStartColor, RGBColor theEndColor );
	void SetRadius( float theRadius );

	void SetPosition( const vector2d & thePosition );
	void AddPositionOffset( const vector2d & theOffset );
	void AddRandomPosition( float min, float max );

	void SetVelocity( const vector2d & theVelocity );
	void AddVelocityOffset( const vector2d & theOffset );
	void AddRandomVelocity( float min, float max );

	void End();

	void Draw();
	void Move();
};

extern ParticleSystemClass ParticleSystem;

#endif


