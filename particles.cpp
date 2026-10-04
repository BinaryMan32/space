#include <math.h>

#include "glstuff.h"
#include "List.h"
#include "timer.h"

#include "particles.h"

float frand()
{
	return ( float( rand() ) / float( RAND_MAX ) );
}

ParticleSystemClass ParticleSystem;

///////////////////////////////////////////////////////////////////////////////
//                                      ///////////////////////////////////////
//  ParticleSystemClass::ParticleGroup  ///////////////////////////////////////
//                                      ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ParticleSystemClass::ParticleGroup::ParticleGroup( int theNumParticles )
{
	NumParticles = theNumParticles;

	Lifetime = 1.0f;
	StartColor = MakeRGBColor( 255, 255, 255, 255 );
	EndColor =   MakeRGBColor( 255, 255, 255, 0   );
	Radius = 5.0f;

	Counter = 0.0f;

	Position = new vector2d [ NumParticles ];
	Velocity = new vector2d [ NumParticles ];
}

ParticleSystemClass::ParticleGroup::~ParticleGroup()
{
	delete [] Position;
	delete [] Velocity;
}

void ParticleSystemClass::ParticleGroup::SetLifetime( float theLifetime )
{
	Lifetime = theLifetime;
}

void ParticleSystemClass::ParticleGroup::SetColor( RGBColor theStartColor, RGBColor theEndColor )
{
	StartColor = theStartColor;
	EndColor = theEndColor;
}

void ParticleSystemClass::ParticleGroup::SetRadius( float theRadius )
{
	Radius = theRadius;
}

void ParticleSystemClass::ParticleGroup::SetPosition( const vector2d & thePosition )
{
	vector2d *PositionPtr = Position + NumParticles;

	while ( --PositionPtr >= Position )
	{
		*PositionPtr = thePosition;
	}
}

void ParticleSystemClass::ParticleGroup::AddPositionOffset( const vector2d & theOffset )
{
	vector2d *PositionPtr = Position + NumParticles;

	while ( --PositionPtr >= Position )
	{
		*PositionPtr += frand() * theOffset;
	}
}

void ParticleSystemClass::ParticleGroup::AddRandomPosition( float min, float max )
{
	vector2d *PositionPtr = Position + NumParticles;

	while ( --PositionPtr >= Position )
	{
		*PositionPtr += ( ( max - min ) * frand() + min ) * vector2d( 2.0f * PI * frand() );
	}
}

void ParticleSystemClass::ParticleGroup::SetVelocity( const vector2d & theVelocity )
{
	vector2d *VelocityPtr = Velocity + NumParticles;

	while ( --VelocityPtr >= Velocity )
	{
		*VelocityPtr = frand() * theVelocity;
	}
}

void ParticleSystemClass::ParticleGroup::AddVelocityOffset( const vector2d & theOffset )
{
	vector2d *VelocityPtr = Velocity + NumParticles;

	while ( --VelocityPtr >= Velocity )
	{
		*VelocityPtr += frand() * theOffset;
	}
}

void ParticleSystemClass::ParticleGroup::AddRandomVelocity( float min, float max )
{
	vector2d *VelocityPtr = Velocity + NumParticles;

	while ( --VelocityPtr >= Velocity )
	{
		*VelocityPtr += ( ( max - min ) * frand() + min ) * vector2d( 2.0f * PI * frand() );
	}
}

void ParticleSystemClass::ParticleGroup::Draw()
{
	float ratio = Counter / Lifetime;

	glColor4ub( StartColor.red   + unsigned char( float( EndColor.red   - StartColor.red   ) * ratio ),
				StartColor.green + unsigned char( float( EndColor.green - StartColor.green ) * ratio ),
				StartColor.blue  + unsigned char( float( EndColor.blue  - StartColor.blue  ) * ratio ),
				StartColor.alpha + unsigned char( float( EndColor.alpha - StartColor.alpha ) * ratio ) );

	vector2d *PositionPtr = Position + NumParticles;

	glBegin( GL_QUADS );

	while ( --PositionPtr >= Position )
	{
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( PositionPtr->x - Radius, PositionPtr->y - Radius );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( PositionPtr->x + Radius, PositionPtr->y - Radius );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( PositionPtr->x + Radius, PositionPtr->y + Radius );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( PositionPtr->x - Radius, PositionPtr->y + Radius );
	}

	glEnd();
}

bool ParticleSystemClass::ParticleGroup::Move()
{
	vector2d *PositionPtr = Position + NumParticles;
	vector2d *VelocityPtr = Velocity + NumParticles;

	while ( --PositionPtr >= Position )
	{
		--VelocityPtr;

		*PositionPtr += *VelocityPtr * FrameTime;
		*VelocityPtr *= ( 1.0f - 0.8f * FrameTime );
	}

	Counter += FrameTime;

	return ( Counter < Lifetime );
}

///////////////////////////////////////////////////////////////////////////////
//                       //////////////////////////////////////////////////////
//  ParticleSystemClass  //////////////////////////////////////////////////////
//                       //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ParticleSystemClass::ParticleSystemClass()
{
	ParticleTexture = NULL;
	GroupPtr = NULL;
}

bool ParticleSystemClass::Init()
{
	ParticleTexture = new glTexture( "particle.tga" );

	return ParticleTexture->Valid();
}

void ParticleSystemClass::Begin( int NumParticles )
{
	if ( GroupPtr == NULL && NumParticles > 0 )
	{
		ParticleGroupList.Insert( GroupPtr = new ParticleGroup( NumParticles ) );
	}
}

void ParticleSystemClass::SetLifetime( float theLifetime )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->SetLifetime( theLifetime );
	}
}

void ParticleSystemClass::SetColor( RGBColor theStartColor, RGBColor theEndColor )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->SetColor( theStartColor, theEndColor );
	}
}

void ParticleSystemClass::SetRadius( float theRadius )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->SetRadius( theRadius );
	}
}

void ParticleSystemClass::SetPosition( const vector2d & thePosition )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->SetPosition( thePosition );
	}
}

void ParticleSystemClass::AddPositionOffset( const vector2d & theOffset )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->AddPositionOffset( theOffset );
	}
}

void ParticleSystemClass::AddRandomPosition( float min, float max )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->AddRandomPosition( min, max );
	}
}

void ParticleSystemClass::SetVelocity( const vector2d & theVelocity )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->SetVelocity( theVelocity );
	}
}

void ParticleSystemClass::AddVelocityOffset( const vector2d & theOffset )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->AddVelocityOffset( theOffset );
	}
}

void ParticleSystemClass::AddRandomVelocity( float min, float max )
{
	if ( GroupPtr != NULL )
	{
		GroupPtr->AddRandomVelocity( min, max );
	}
}

void ParticleSystemClass::End()
{
	if ( GroupPtr != NULL )
	{
		GroupPtr = NULL;
	}
}

void ParticleSystemClass::Draw()
{
	GLint glTextureFunction;
	glGetTexEnviv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &glTextureFunction );
	
	if ( glTextureFunction != GL_MODULATE ) glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		GLboolean glTexturesEnabled;
		glGetBooleanv( GL_TEXTURE_2D, &glTexturesEnabled );

		if ( ! glTexturesEnabled ) glEnable( GL_TEXTURE_2D );

			ParticleTexture->Select( 0 );

			List<ParticleGroup *>::Iterator ParticleGroupIterator = ParticleGroupList.Start();
			
			while ( ++ParticleGroupIterator )
			{
				ParticleGroupIterator->Draw();
			}

		if ( ! glTexturesEnabled ) glDisable( GL_TEXTURE_2D );

	if ( glTextureFunction != GL_MODULATE ) glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glTextureFunction );
}

void ParticleSystemClass::Move()
{
	List<ParticleGroup *>::Iterator ParticleGroupIterator = ParticleGroupList.Start();

	while ( ++ParticleGroupIterator )
	{
		if ( ! ParticleGroupIterator->Move() )
		{
			delete *ParticleGroupIterator;
			ParticleGroupList.Remove( ParticleGroupIterator-- );
		}
	}
}

