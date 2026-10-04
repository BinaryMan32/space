#include "objects.h"

#include "StarField.h"
#include "DirectInput.h"

//	Camera	///////////////////////////////////////////////////////////////////////////////////

Camera::Camera() : SpriteNode()
{
	AngularVelocity = 0.5;
	States[ New    ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, 0, Normal );

	Stars.Init( 500 );

	SetDrag( 5.0f );
}

void Camera::OnTick()
{
	// Rotate view to Player's angle
	glRotatef( 90.0f - Degrees( Angle ), 0.0f, 0.0f, 1.0f );
	
	// Draw the StarField before translation
	Stars.Move( Position.x, Position.y );
	Stars.Draw();
	
	// Translate view to Player's position
	glTranslatef( -Position.x, -Position.y, 0.0f );
}

//	Asteroid  /////////////////////////////////////////////////////////////////////////////////

float Asteroid::MassArray[4] = { 50, 100, 200, 400 };
float Asteroid::RadiusArray[4] = { 8, 16, 24, 32 };
char *Asteroid::ImageArray[4] = { "SmallAsteroid.tga", "MediumAsteroid.tga", "LargeAsteroid.tga", "HugeAsteroid.tga" };

Asteroid::Asteroid( int theSize ) : SpriteNode( ImageArray[ theSize ] )
{
	SetMass( MassArray[ theSize ] );
	
	Radius = RadiusArray[ theSize ];
	size = theSize;

	States[ New    ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, 1, Normal );
	States[ Injury ].Init( 0, 1, Normal );
	States[ Death  ].Init( 0, 1, Dead   );

	MaxLife = Life = 10.0f;
}

void Asteroid::OnTick()
{

}

void Asteroid::OnStateChange()
{
	if ( CurrentState == Dead )
	{
		Sprite NewSprite;
		
		if ( size >= 2 )
			NewSprite = GetInterface().Master()->AddSprite( new LargeExplosion(), LargeExplosionID );
		else
			NewSprite = GetInterface().Master()->AddSprite( new SmallExplosion(), SmallExplosionID );
		
		NewSprite->SetPosition( Position, Radians( float( rand()%360 ) ) );
		NewSprite->SetVelocity( Velocity, 0.0f );
		
		if ( size > 0 )
		{
			int RemainingSize = size * 3;
			
			while ( RemainingSize > 0 )
			{
				int MaxSize = ( RemainingSize >= size ) ? ( size ) : ( RemainingSize );

				int NewSize = ( MaxSize > 0 ) ? ( rand()%( MaxSize ) ) : ( 0 );

				NewSprite = GameWorld.AddSprite( new Asteroid( NewSize ), AsteroidID );

				NewSprite->SetPosition(
					Position + float( rand()%int(0.75f*Radius) ) * vector2d( Radians( float( rand()%360 ) ) ),
					Radians( float( rand()%360 ) ) );
			
				NewSprite->SetVelocity(
					Velocity + float( 25 + rand()%75 ) * vector2d( Radians( float( rand()%360 ) ) ),
					Radians( float( rand()%91 - 45 ) ) );

				RemainingSize -= NewSize + 1;
			}
		}
	
		if ( rand()%50 == 0 )
		{
			NewSprite = GameWorld.AddSprite( new Health(), HealthID );

			NewSprite->SetPosition(
				Position + float( rand()%int(0.75f*Radius) ) * vector2d( Radians( float( rand()%360 ) ) ),
				Radians( float( rand()%360 ) ) );

			NewSprite->SetVelocity(
				Velocity + float( 25 + rand()%75 ) * vector2d( Radians( float( rand()%360 ) ) ),
				Radians( float( rand()%91 - 45 ) ) );
		}
	}
}

//	PlayerShip	///////////////////////////////////////////////////////////////////////////////

PlayerShip::PlayerShip() : SpriteNode( "PlayerShip.tga" )
{
	States[ Birth  ].Init(  0, 3, Normal );
	States[ Normal ].Init(  3, 1, Normal );
	States[ Move   ].Init(  4, 3, Move );
	States[ Attack ].Init(  7, 3, Normal );
	States[ Injury ].Init( 10, 3, Normal, "PlayerShipInjury.wav" );
	States[ Death  ].Init(  0, 0, Dead, "PlayerShipDeath.wav" );

	SetMass( 100.0f );
	SetDrag( 0.9f );

	MaxLife = Life = 100.0f;
	MaxGunEnergy = GunEnergy = 5.0f;
	MaxFireDelay = FireDelay = 0.125f;

	QuadricObject = gluNewQuadric();
}

PlayerShip::~PlayerShip()
{
	gluDeleteQuadric( QuadricObject );
}

void PlayerShip::OnStateChange()
{
	if ( CurrentState == Dead )
	{
		Sprite NewSprite = GetInterface().Master()->AddSprite( new LargeExplosion(), LargeExplosionID );
		NewSprite->SetPosition( Position, Angle );
		NewSprite->SetVelocity( Velocity, 0.0f );
	
		for ( int index = 0; index < 8; index++ )
		{
			NewSprite = GetInterface().Master()->AddSprite( new SmallExplosion(), SmallExplosionID );

			NewSprite->SetPosition(
				Position + float( rand()%int(0.75f*Radius) ) * vector2d( Radians( float( rand()%360 ) ) ),
				Radians( float( rand()%360 ) ) );
		
			NewSprite->SetVelocity( Velocity + float( 25 + rand()%75 ) * vector2d( Radians( float( rand()%360 ) ) ), 0.0f );
		}

		ParticleSystem.Begin( 512 );
			ParticleSystem.SetLifetime( 2.0f );
			ParticleSystem.SetRadius( 32.0f );
			ParticleSystem.SetColor( MakeRGBColor( 255, 255, 0, 255 ), MakeRGBColor( 255, 0, 0, 0 ) );
			ParticleSystem.SetPosition( Position );
			ParticleSystem.SetVelocity( Velocity );
			ParticleSystem.AddRandomVelocity( 800.0f, 850.0f );
		ParticleSystem.End();
	}
}

bool PlayerShip::OnCollide( SpriteBase *otherSprite )
{
	return true;
}

void PlayerShip::OnTick()
{
	static float OldMouseAngularAcceleration = 0.0f;
	
	float MouseAngularAcceleration =
		( FrameTime > 0.0f ) ? ( -1.5f * DInput.dx ) : ( 0.0f );
	
	AngularAcceleration += ( -DInput.dx + OldMouseAngularAcceleration ) / 2.0f;
	
	OldMouseAngularAcceleration = MouseAngularAcceleration;

	if ( DInput.KeyDown( DIK_UPARROW ) )
	{
		Accelerate(  750 * vector2d( Angle ), 0 );

		ParticleSystem.Begin( int( FrameTime / 0.005f ) + 1 );
			ParticleSystem.SetLifetime( 0.125f );
			ParticleSystem.SetRadius( 6.0f );
			ParticleSystem.SetColor( MakeRGBColor( 255, 255, 0, 255 ), MakeRGBColor( 255, 0, 0, 0 ) );
			ParticleSystem.SetPosition( Position - 24.0f * vector2d( Angle ) );
			ParticleSystem.AddPositionOffset( OldPosition - Position );
			ParticleSystem.SetVelocity( vector2d( 0.0f, 0.0f ) );
			ParticleSystem.AddRandomVelocity( 0.0f, 20.0f );
		ParticleSystem.End();
	}

	if ( DInput.KeyDown( DIK_DOWNARROW  ) ) Accelerate( -300 * vector2d( Angle ), 0 );
	if ( DInput.KeyDown( DIK_LEFTARROW  ) ) Accelerate(  300 * Perpendicular( vector2d( Angle ) ), 0 );
	if ( DInput.KeyDown( DIK_RIGHTARROW ) ) Accelerate( -300 * Perpendicular( vector2d( Angle ) ), 0 );

	if ( DInput.KeyDown( DIK_LEFTMOUSE ) && GunEnergy > 0.25f && FireDelay <= 0.0f )
	{
		if ( SetState( Attack ) )
		{
			Sprite NewSprite = GetInterface().Master()->AddSprite( new RedLaser(), RedLaserID );
			NewSprite->SetPosition( Position +  16 * vector2d( Angle ), Angle );
			NewSprite->SetVelocity( Velocity + 600 * vector2d( Angle ), 0.0f  );

			Velocity -= 50.0f * vector2d( Angle );

			GunEnergy -= 0.25f;
			FireDelay = MaxFireDelay;
		}
	}

	if ( DInput.KeyDown( DIK_RIGHTMOUSE ) && GunEnergy > 0.5f && FireDelay <= 0.0f )
	{
		if ( SetState( Attack ) )
		{
			World *WorldInterface = (World *)(GetInterface().Master());
			WorldInterface->DisableTypes();
			WorldInterface->EnableType( AsteroidID );

			Sprite TargetSprite = WorldInterface->GetClosestTypeFront( GetInterface() );

			Sprite NewSprite = GetInterface().Master()->AddSprite( new RedLaser(), RedLaserID );
			NewSprite->SetPosition( Position +  16 * vector2d( Angle ), Angle );
			NewSprite->SetVelocity( Velocity + 600 * vector2d( Angle ), 0.0f  );

			GetInterface().Master()->AddAction( NewSprite, TargetSprite, new FollowSprite() );

			GunEnergy -= 0.5f;
			FireDelay = MaxFireDelay;
		}
	}

	if ( ( GunEnergy += FrameTime ) > MaxGunEnergy ) GunEnergy = MaxGunEnergy;

	if ( ( FireDelay -= FrameTime ) < 0.0f ) FireDelay = 0.0f;
}

void PlayerShip::OnDraw()
{

}

void PlayerShip::PostDraw()
{
	glPushMatrix();

		glTranslatef( 325.0f, -125.0f, 0.0f );
	
		gluQuadricDrawStyle( QuadricObject, GLU_FILL );

		if ( CurrentState == Attack )
		{
			glColor4f( 1.0f - GunEnergy / MaxGunEnergy, 0.0f, GunEnergy / MaxGunEnergy, 0.5f );
			gluPartialDisk( QuadricObject, 22.0, 28.0, int( 50.0f * GunEnergy / MaxGunEnergy ), 1, -90.0, 360.0 * GunEnergy / MaxGunEnergy );
		}

		if ( CurrentState == Injury )
		{
			glColor4f( 1.0f - Life / MaxLife, Life / MaxLife, 0.0f, 0.5f );
			gluPartialDisk( QuadricObject, 0.0, 18.0, int( 50.0f * Life / MaxLife ), 1, -90.0, 360.0 * Life / MaxLife );
		}

		gluQuadricDrawStyle( QuadricObject, GLU_SILHOUETTE );

		glColor4f( 1.0f - GunEnergy / MaxGunEnergy, 0.0f, GunEnergy / MaxGunEnergy, 1.0f );
		gluPartialDisk( QuadricObject, 22.0, 28.0, int( 50.0f * GunEnergy / MaxGunEnergy ), 1, -90.0, 360.0 * GunEnergy / MaxGunEnergy );

		glColor4f( 1.0f - Life / MaxLife, Life / MaxLife, 0.0f, 1.0f );
		gluPartialDisk( QuadricObject, 0.0, 18.0, int( 50.0f * Life / MaxLife ), 1, -90.0, 360.0 * Life / MaxLife );

	glPopMatrix();
}

//	Health	///////////////////////////////////////////////////////////////////////////////////

Health::Health() : SpriteNode( "health.tga" )
{
	States[ New    ].Init( 0, 0, Birth );
	States[ Birth  ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, NumFrames, Normal );
	States[ Death  ].Init( 0, 0, Dead, "Health.wav" );

	SetMass( 100.0f );
	SetDrag( 1.0f );

	MaxLife = Life = 1000.0f;
}

void Health::OnTick()
{

}

bool Health::OnCollide( SpriteBase *otherSprite )
{
	if ( ((SpriteNode *)otherSprite)->GetInterface().Type().TypeID() == PlayerShipID )
	{
		otherSprite->SetLife( otherSprite->GetLife() + 10.0f );
		SetState( Death );

		Sprite NewSprite = GetInterface().Master()->AddSprite( new Flare(), FlareID );
		NewSprite->SetPosition( GetPosition(), Radians( float( rand()%360 ) ) );
		
		return false;
	}

	return true;
}

//	RedLaser	///////////////////////////////////////////////////////////////////////////////

RedLaser::RedLaser() : SpriteNode( "RedLaser.tga" )
{
	SetMass( 25.0f );

	States[ New    ].Init( 0, 0, Birth );
	States[ Birth  ].Init( 0, 0, Normal, "RedLaserBirth.wav" );
	States[ Normal ].Init( 0, NumFrames, Normal );
	States[ Death  ].Init( 0, 0, Dead, "RedLaserDeath.wav" );

	MaxLife = Life = 10.0f;
	Radius = 8.0f;

	TimeToLive = 1.5f;
	TrailTime = 0.0f;
}

void RedLaser::OnStateChange()
{
	if ( CurrentState == Dead )
	{
		Sprite NewSprite = GetInterface().Master()->AddSprite( new RedExplosion(), RedExplosionID );
		NewSprite->SetPosition( Position, Radians( float( rand()%360 ) ) );
		NewSprite->SetVelocity( Velocity, 0.0f );
	}
}

void RedLaser::OnTick()
{
	int NumParticles = 0;
	TrailTime += FrameTime;
	
	while ( TrailTime > 0.0075f )
	{
		TrailTime -= 0.0075f;
		NumParticles++;
	}
		
	if ( NumParticles > 0 )
	{
		ParticleSystem.Begin( NumParticles );
			ParticleSystem.SetLifetime( 1.0f );
			ParticleSystem.SetRadius( 14.0f );
			ParticleSystem.SetColor( MakeRGBColor( 255, 0, 0, 127 ), MakeRGBColor( 192, 192, 192, 0 ) );
			ParticleSystem.SetPosition( Position );
			ParticleSystem.AddPositionOffset( Position - OldPosition );
			ParticleSystem.AddRandomPosition( 0.0f, Radius / 2 );
			ParticleSystem.SetVelocity( vector2d( 0.0f, 0.0f ) );
			ParticleSystem.AddRandomVelocity( 0.0f, 15.0f );
		ParticleSystem.End();
	}

	if ( ( TimeToLive -= FrameTime ) <= 0.0f ) SetState( Dead );
}

//	RedExplosion	///////////////////////////////////////////////////////////////////////////

RedExplosion::RedExplosion() : SpriteNode( "RedExplosion.tga" )
{
	States[ New    ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, NumFrames, Dead );

	FrameLength = FrameCounter = 0.05f;
}

void RedExplosion::OnStateChange()
{
	if ( CurrentState == Normal )
	{
		Velocity = Limit( Velocity, 50.0f );
	}
}

void RedExplosion::OnTick()
{

}

//	SmallExplosion	///////////////////////////////////////////////////////////////////////////

SmallExplosion::SmallExplosion() : SpriteNode( "SmallExplosion.tga" )
{
	States[ New    ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, NumFrames, Dead );

	FrameLength = FrameCounter = 0.05f;
}

void SmallExplosion::OnStateChange()
{

}

void SmallExplosion::OnTick()
{

}

//	LargeExplosion	///////////////////////////////////////////////////////////////////////////

LargeExplosion::LargeExplosion() : SpriteNode( "LargeExplosion.tga" )
{
	States[ New    ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, NumFrames, Dead );

	FrameLength = FrameCounter = 0.05f;
}

void LargeExplosion::OnStateChange()
{

}

void LargeExplosion::OnTick()
{

}

//	Flare	///////////////////////////////////////////////////////////////////////////////////

Flare::Flare() : SpriteNode( "flare.tga" )
{
	States[ New ].Init( 0, 0, Normal );
	States[ Normal ].Init( 0, 1, Normal );

	ImageSize = 16.0f;
	Radius = ImageSize * 1.414f;
	RadialVelocity = 200.0f;
}

void Flare::OnTick()
{
	RadialAcceleration = -1000.0f;
	if ( Radius <= 0.0f ) SetState( Dead );
}

//	FollowSprite	///////////////////////////////////////////////////////////////////////////

void CameraLock::OnTick()
{
	Action ActionInterface = GetInterface();

	Sprite Source = ActionInterface.Source();
	Sprite Target = ActionInterface.Target();

	Source->Accelerate( 25.0f * ( Target->GetPosition() - Source->GetPosition() ),
						75.0f * ( Target->GetAngle() - Source->GetAngle() ) );
}

void FollowSprite::OnTick()
{
	Action ActionInterface = GetInterface();

	Sprite Source = ActionInterface.Source();
	Sprite Target = ActionInterface.Target();

	vector2d RelativePosition = Target->GetPosition() - Source->GetPosition();
	vector2d RelativeVelocity = Target->GetVelocity() - Source->GetVelocity();
	
	vector2d Direction;
	
	if ( DotP( RelativePosition, RelativeVelocity ) < 0 )
	{
		float MovementTime = Mag( RelativePosition ) / Mag( RelativeVelocity );
		Direction = ( RelativePosition ) / MovementTime + Target->GetVelocity();
	}
	else
	{
		Direction = RelativePosition;
	}

	Source->Accelerate( Scale( Direction, 25000.0f ), 0.0f );
	Source->SetVelocity( Limit( Source->GetVelocity(), 600.0f ), Source->GetAngularVelocity() );
}
