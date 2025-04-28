#include "spritebase.h"

bool StateInterruptionAllowed[ NUM_SPRITE_STATES ][ NUM_SPRITE_STATES ] = {
/* OldState								NewState								*/
/*   \/			New		Birth	Normal	Move	Attack	Injury	Death	Dead	*/
/* New    */	0,		0,		0,		0,		0,		0,		0,		0,
/* Birth  */	0,		0,		0,		0,		0,		0,		0,		0,
/* Normal */	0,		0,		1,		1,		1,		1,		1,		1,
/* Move   */	0,		0,		1,		1,		1,		1,		1,		1,
/* Attack */	0,		0,		1,		0,		1,		1,		1,		1,
/* Injury */	0,		0,		1,		1,		0,		1,		1,		1,
/* Death  */	0,		0,		0,		0,		0,		0,		0,		1,
/* Dead   */	0,		0,		0,		0,		0,		0,		0,		0
};

SpriteBase::SpriteBase() : Image()
{
	Angle = 0;
	Radius = 0;

	AngularVelocity = 0;
	RadialVelocity = 0;

	AngularAcceleration = 0;
	RadialAcceleration = 0;

	Torque = 0;

	Mass = 0;
	MomentOfInertia = 0;

	DragConstant = 0;
	
	Life = 0;
	MaxLife = 0;

	ImageSize = 0;

	FrameCounter = FrameLength = 0.15f;

	CurrentFrame = 0;
	NumFrames = 0;

	CurrentState = New;

	States[ New ].Init( 0, 0, Birth );
	States[ Dead ].Init( 0, 0, Dead );
}

SpriteBase::SpriteBase( string FileName ) : Image( FileName )
{
	Angle = 0;
	Radius = float( ( Image.GetWidth() + Image.GetHeight() ) / 4 );

	AngularVelocity = 0;
	RadialVelocity = 0;

	AngularAcceleration = 0;
	RadialAcceleration = 0;

	Torque = 0;

	Mass = 0;
	MomentOfInertia = 0;

	DragConstant = 0;

	Life = 0;
	MaxLife = 0;

	ImageSize = float( Image.GetWidth() + Image.GetHeight() ) / 4.0f;

	FrameCounter = FrameLength = 0.15f;

	CurrentFrame = 0;
	NumFrames = Image.GetNumFrames();

	CurrentState = New;

	States[ New ].Init( 0, 0, Birth );
	States[ Dead ].Init( 0, 0, Dead );
}

SpriteBase::~SpriteBase()
{

}

float SpriteBase::GetMass() const
{
	return Mass;
}

void SpriteBase::SetMass( float theMass )
{
	Mass = theMass;
	MomentOfInertia = 0.5f * theMass * Radius * Radius;
}

float SpriteBase::GetLife() const
{
	return Life;
}

float SpriteBase::SetLife( float theLife )
{
	return ( ( theLife <= MaxLife ) ? Life = theLife : Life = MaxLife );
}

float SpriteBase::GetMaxLife() const
{
	return MaxLife;
}

void SpriteBase::SetMaxLife( float theMaxLife )
{
	MaxLife = theMaxLife;
	if ( Life > MaxLife ) Life = MaxLife;
}

float SpriteBase::GetRadius() const
{
	return Radius;
}

SpriteStateIndex SpriteBase::GetState()
{
	return CurrentState;
}

bool SpriteBase::SetState( SpriteStateIndex NewState )
{
	if ( ( NewState >= New ) && ( NewState <= Dead ) )
	{
		if ( StateInterruptionAllowed[ CurrentState ][ NewState ] )
		{
			if ( States[ NewState ].Valid() )
			{
				CurrentState = NewState;
				CurrentFrame = States[ CurrentState ].GetStartFrame();
				FrameCounter = FrameLength;
				States[ CurrentState ].PlaySound();
				OnStateChange();
				return true;
			}
		}
	}

	return false;
}

float SpriteBase::GetAngle()
{
	return Angle;
}

vector2d SpriteBase::GetPosition()
{
	return Position;
}

void SpriteBase::SetPosition( vector2d thePosition, float theAngle )
{
	OldPosition = Position = thePosition;
	Angle = theAngle;
}

float SpriteBase::GetAngularVelocity()
{
	return AngularVelocity;
}

vector2d SpriteBase::GetVelocity()
{
	return Velocity;
}

void SpriteBase::SetVelocity( vector2d theVelocity, float theAngularVelocity )
{
	Velocity = theVelocity;
	AngularVelocity = theAngularVelocity;
}

void SpriteBase::Accelerate( vector2d theAcceleration, float theAngularAcceleration )
{
	Acceleration += theAcceleration;
	AngularAcceleration += theAngularAcceleration;
}

void SpriteBase::AddForce( vector2d theForce )
{
	Force += theForce;
}

void SpriteBase::AddForce( vector2d theForce, vector2d theOffset )
{
	Force += theForce;
	Torque = CrossP( theOffset, theForce );
}

void SpriteBase::Gravity( SpriteBase *theSprite, float theConstant )
{
	if ( theSprite != NULL )
	{
		vector2d Distance = theSprite->Position - Position;

		if ( ( Distance.x != 0 ) || ( Distance.y != 0 ) )
		{
			vector2d Gravity = theConstant * Mass * theSprite->Mass / MagSquared( Distance );
			AddForce( Gravity );
			theSprite->AddForce( -Gravity );
		}
	}
}

void SpriteBase::SetDrag( float theDragConstant )
{
	DragConstant = theDragConstant;
}

void SpriteBase::Tick()
{
	float HalfTimeSquared = 0.5f * FrameTime * FrameTime;
	float OldRadius = Radius;

	// convert forces to accelerations
	if ( Mass != 0 ) Acceleration += Force / Mass;
	if ( MomentOfInertia != 0 ) AngularAcceleration += Torque / MomentOfInertia;

	// position
	OldPosition = Position;
	Position += Velocity * FrameTime + Acceleration * HalfTimeSquared;
	Radius += RadialVelocity * FrameTime + RadialAcceleration * HalfTimeSquared;
	ImageSize *= Radius / OldRadius;
	Angle += AngularVelocity * FrameTime + AngularAcceleration * HalfTimeSquared;
	NormalizeAngle( Angle );

	// velocity
	Velocity += Acceleration * FrameTime;
	RadialVelocity += RadialAcceleration * FrameTime;
	AngularVelocity += AngularAcceleration * FrameTime;
	
	// drag
	Velocity *= ( 1.0f - ( DragConstant * FrameTime ) );
	AngularVelocity *= ( 1.0f - ( DragConstant * FrameTime * 6.2832f ) );

	// reset accumulator values
	Acceleration = vector2d( 0, 0 );
	RadialAcceleration = 0;
	AngularAcceleration = 0;
	Force = vector2d( 0, 0 );
	Torque = 0;

	OnTick();

	FrameCounter -= FrameTime;
	
	if ( CurrentFrame >= States[ CurrentState ].GetEndFrame() ) FrameCounter = 0;
	
	if ( FrameCounter <= 0 ) 
	{
		CurrentFrame++;
		FrameCounter += FrameLength;

		if ( CurrentFrame >= States[ CurrentState ].GetEndFrame() )
		{
			SpriteStateIndex NewState = States[ CurrentState ].GetNextState();
			
			if ( ( NewState >= New ) && ( NewState <= Dead ) )
			{
				CurrentState = NewState;
				CurrentFrame = States[ CurrentState ].GetStartFrame();
				States[ CurrentState ].PlaySound();
				OnStateChange();
			}
		}
	}
}

void SpriteBase::Draw()
{
	if ( ( CurrentState > New ) && ( CurrentState < Dead ) && Image )
	{
		glPushMatrix();

			glTranslatef( Position.x, Position.y, 0.0f );
			glRotatef( Degrees( Angle ), 0.0f, 0.0f, 1.0f );
			
			Image.Select( CurrentFrame );

			glBegin( GL_QUADS );	
				glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -ImageSize, -ImageSize );	
				glTexCoord2f( 0.0f, 1.0f ); glVertex2f( -ImageSize,  ImageSize );
				glTexCoord2f( 1.0f, 1.0f ); glVertex2f(  ImageSize,  ImageSize );
				glTexCoord2f( 1.0f, 0.0f ); glVertex2f(  ImageSize, -ImageSize );
			glEnd();

		glPopMatrix();

		OnDraw();
	}
}

void SpriteBase::OnTick()
{
	
}

void SpriteBase::OnStateChange()
{

}

bool SpriteBase::OnCollide( SpriteBase *otherSprite )
{
	return true;
}

void SpriteBase::OnDraw()
{

}

void SpriteBase::PostDraw()
{

}

void SpriteCollision( SpriteBase *A, SpriteBase *B )
{
	if ( ( A->CurrentState == New   ) || ( B->CurrentState == New   ) ) return;
	if ( ( A->CurrentState == Dead  ) || ( B->CurrentState == Dead  ) ) return;

	// Calculate normal vector
	vector2d Normal( A->Position - B->Position );
	
	// Calculate distance squared
	float NormalDotNormal = MagSquared( Normal );

	// No collision if distance is greater than sum of radii
	if ( NormalDotNormal > ( ( A->Radius + B->Radius ) * ( A->Radius + B->Radius ) ) )
		return;

	// Notify objects that collision occurred
	if ( !A->OnCollide( B ) || !B->OnCollide( A ) )
		return;
	
	// No collision if objects are massless
	if ( ( A->Mass == 0 ) || ( B->Mass == 0 ) )
		return;

	// Calculate delta velocity vector
	vector2d DeltaV( A->Velocity - B->Velocity );
	
	// Velocity in the direction of the normal vector
	float DeltaVDotNormal = DotP( DeltaV, Normal );
	
	// No collision if objects are moving away from each other
	if ( DeltaVDotNormal >= 0.0f )
		return;

	// Elasticity constant
	float e = 0.9f;

	// Calculate inverse masses
	float MassInvA = 1 / A->Mass;
	float MassInvB = 1 / B->Mass;
	
	// Calculate impulse
	float impulse = - ( ( 1 + e ) * DeltaVDotNormal ) / ( NormalDotNormal * ( MassInvA + MassInvB ) );

	// Update linear velocities
	A->Velocity += ( impulse * MassInvA ) * Normal;
	B->Velocity -= ( impulse * MassInvB ) * Normal;
	
	// Update angular velocities
	A->AngularVelocity += 0.25f * CrossP(  Scale( Normal, A->Radius ), DeltaV ) * ( B->Mass / A->MomentOfInertia );
	B->AngularVelocity -= 0.25f * CrossP( -Scale( Normal, B->Radius ), DeltaV ) * ( A->Mass / B->MomentOfInertia );

	// Calculate object damage
	DeltaVDotNormal /= Mag( Normal );
	A->Life -= 0.00015f * B->Mass * MassInvA * DeltaVDotNormal * DeltaVDotNormal;
	B->Life -= 0.00015f * A->Mass * MassInvB * DeltaVDotNormal * DeltaVDotNormal;

	A->SetState( A->Life > 0 ? Injury : Death );
	B->SetState( B->Life > 0 ? Injury : Death );
}
