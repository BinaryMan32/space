#ifndef SPRITEBASE_H
#define SPRITEBASE_H

#include "Sound.h"
#include "glstuff.h"
#include "timer.h"
#include "vector.h"

class SpriteBase;

extern void SpriteCollision( SpriteBase *A, SpriteBase *B );

#define NUM_SPRITE_STATES	8

extern bool StateInterruptionAllowed[ NUM_SPRITE_STATES ][ NUM_SPRITE_STATES ];

typedef enum SpriteStateIndex { New = 0, Birth, Normal, Move, Attack, Injury, Death, Dead };

class SpriteState
{
	private:

	bool Enabled;
	int StartFrame;
	int EndFrame;
	SpriteStateIndex NextState;
	Sound StartSound;

	public:
	
	SpriteState()
	{
		Enabled = false;
	}

	void Init( int theStartFrame, int theNumFrames, SpriteStateIndex theNextState )
	{
		if ( ( theStartFrame >= 0 ) && ( theNumFrames >= 0 ) )
		{
			if ( ( theNextState >= New ) && ( theNextState <= Dead ) )
			{
				Enabled = true;
				StartFrame = theStartFrame;
				EndFrame = theStartFrame + theNumFrames;
				NextState = theNextState;
			}
		}
	}

	void Init( int theStartFrame, int theNumFrames, SpriteStateIndex theNextState, char *theSoundName )
	{
		if ( ( theStartFrame >= 0 ) && ( theNumFrames >= 0 ) )
		{
			if ( ( theNextState >= New ) && ( theNextState <= Dead ) )
			{
				Enabled = true;
				StartFrame = theStartFrame;
				EndFrame = theStartFrame + theNumFrames;
				NextState = theNextState;
				StartSound = Sound( theSoundName );
			}
		}
	}

	int GetStartFrame()
	{
		return StartFrame;
	}

	int GetEndFrame()
	{
		return EndFrame;
	}

	SpriteStateIndex GetNextState()
	{
		return NextState;
	}

	bool PlaySound()
	{
		return StartSound.Play();
	}
	
	bool Valid()
	{
		return Enabled;
	}
};

class SpriteBase
{
	protected:
	
	// physical properties
	vector2d Position;
	vector2d OldPosition;
	float Angle;
	float Radius;

	vector2d Velocity;
	float AngularVelocity;
	float RadialVelocity;

	vector2d Acceleration;
	float AngularAcceleration;
	float RadialAcceleration;

	vector2d Force;
	float Torque;

	float Mass;
	float MomentOfInertia;
	float DragConstant;

	float Life;
	float MaxLife;

	// image variables
	glTexture Image;
	float ImageSize;

	float FrameCounter;
	float FrameLength;

	int CurrentFrame;
	int NumFrames;
	
	// sprite state variables
	SpriteStateIndex CurrentState;
	SpriteState States[ NUM_SPRITE_STATES ];

	public:

	SpriteBase();
	SpriteBase( string FileName );
	virtual ~SpriteBase();

	float GetMass() const;
	void SetMass( float theMass );
	
	float GetLife() const;
	float SetLife( float theLife );

	float GetMaxLife() const;
	void SetMaxLife( float theMaxLife );
	
	float GetRadius() const;
	
	SpriteStateIndex GetState();
	bool SetState( SpriteStateIndex NewState );
	
	float GetAngle();
	vector2d GetPosition();
	void SetPosition( vector2d thePosition, float theAngle );

	float GetAngularVelocity();
	vector2d GetVelocity();
	void SetVelocity( vector2d theVelocity, float theAngularVelocity );

	void Accelerate( vector2d theAcceleration, float theAngularAcceleration );
	
	void AddForce( vector2d theForce );
	void AddForce( vector2d theForce, vector2d theOffset );

	void Gravity( SpriteBase *theSprite, float theConstant = 1000.0f );

	void SetDrag( float theDragConstant );

	void Tick();
	void Draw();

	virtual void OnTick() = 0;
	virtual void OnStateChange();
	virtual bool OnCollide( SpriteBase *otherSprite );
	virtual void OnDraw();
	virtual void PostDraw();

	friend void SpriteCollision( SpriteBase *A, SpriteBase *B );
};

#endif
