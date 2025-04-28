#include "world.h"
#include "glstream.h"

World GameWorld;

World::World()
{
	MaxRadius = 32.0f;
	QuadricObject = gluNewQuadric();
}

World::~World()
{
	gluDeleteQuadric( QuadricObject );
}

Sprite World::GetClosest( Sprite theSprite )
{
	if ( !theSprite.Valid() ) return Sprite();
	
	float TempDistanceSquared;
	float DistanceSquared = 1000000000000.0f;
	float Distance = 1000000.0f;

	vector2d StartPosition = theSprite->GetPosition();

	Sprite ClosestSprite;

	Sprite CurrentSprite = theSprite;
	while ( --CurrentSprite )
	{
		if ( CurrentSprite->GetMass() > 0.0f )
		{
			TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

			if ( TempDistanceSquared < DistanceSquared )
			{
				DistanceSquared = TempDistanceSquared;
				Distance = fsqrt( TempDistanceSquared );
				ClosestSprite = CurrentSprite;
			}
			else
			{
				if ( Distance < ( CurrentSprite->GetPosition().y - StartPosition.y ) )
					break;
			}
		}
	}

	CurrentSprite = theSprite;
	while ( ++CurrentSprite )
	{
		if ( CurrentSprite->GetMass() > 0.0f )
		{
			TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

			if ( TempDistanceSquared < DistanceSquared )
			{
				DistanceSquared = TempDistanceSquared;
				Distance = fsqrt( TempDistanceSquared );
				ClosestSprite = CurrentSprite;
			}
			else
			{
				if ( Distance < ( StartPosition.y - CurrentSprite->GetPosition().y ) )
					break;
			}
		}
	}

	return ClosestSprite;
}
	
Sprite World::GetClosestType( Sprite theSprite )
{
	if ( !theSprite.Valid() ) return Sprite();

	SpriteType CurrentType = FirstSpriteType();
	Sprite CurrentSprite;

	vector2d StartPosition = theSprite->GetPosition();

	Sprite ClosestSprite;

	float TempDistanceSquared;
	float DistanceSquared = 1000000000000.0f;

	while ( CurrentType )
	{
		if ( CurrentType.Enabled() )
		{
			CurrentSprite = CurrentType.FirstSprite();

			if ( CurrentType == theSprite.Type() )
			{
				while ( CurrentSprite )
				{
					if ( CurrentSprite != theSprite )
					{
						TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

						if ( TempDistanceSquared < DistanceSquared )
						{
							DistanceSquared = TempDistanceSquared;
							ClosestSprite = CurrentSprite;
						}
					}

					CurrentSprite = CurrentSprite.TypeNext();
				}
			}
			else
			{
				while ( CurrentSprite )
				{
					TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

					if ( TempDistanceSquared < DistanceSquared )
					{
						DistanceSquared = TempDistanceSquared;
						ClosestSprite = CurrentSprite;
					}

					CurrentSprite = CurrentSprite.TypeNext();
				}
			}
		}

		CurrentType++;
	}

	return ClosestSprite;
}

Sprite World::GetClosestTypeFront( Sprite theSprite )
{
	if ( !theSprite.Valid() ) return Sprite();

	SpriteType CurrentType = FirstSpriteType();
	Sprite CurrentSprite;

	vector2d StartPosition = theSprite->GetPosition();
	vector2d StartDirection = vector2d( theSprite->GetAngle() );

	Sprite ClosestSprite;

	float TempDistanceSquared;
	float DistanceSquared = 1000000000000.0f;

	while ( CurrentType )
	{
		if ( CurrentType.Enabled() )
		{
			CurrentSprite = CurrentType.FirstSprite();

			if ( CurrentType == theSprite.Type() )
			{
				while ( CurrentSprite )
				{
					if ( CurrentSprite != theSprite )
					{
						TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

						if ( TempDistanceSquared < DistanceSquared )
						{
							if ( DotP( CurrentSprite->GetPosition() - StartPosition, StartDirection ) > 0.0f )
							{
								DistanceSquared = TempDistanceSquared;
								ClosestSprite = CurrentSprite;
							}
						}
					}

					CurrentSprite = CurrentSprite.TypeNext();
				}
			}
			else
			{
				while ( CurrentSprite )
				{
					TempDistanceSquared = MagSquared( CurrentSprite->GetPosition() - StartPosition );

					if ( TempDistanceSquared < DistanceSquared )
					{
						if ( DotP( CurrentSprite->GetPosition() - StartPosition, StartDirection ) > 0.0f )
						{
							DistanceSquared = TempDistanceSquared;
							ClosestSprite = CurrentSprite;
						}
					}

					CurrentSprite = CurrentSprite.TypeNext();
				}
			}
		}

		CurrentType++;
	}

	return ClosestSprite;
}

void World::SortSprites()
{
	Sprite CurrentSprite;
	Sprite OtherSprite;

	float MaxPositionY;
	float CurrentPositionY;

	CurrentSprite = FirstSprite();

	if ( CurrentSprite )
	{
		MaxPositionY = CurrentSprite->GetPosition().y;
		CurrentSprite++;

		while ( CurrentSprite )
		{
			if ( CurrentSprite->GetPosition().y >= MaxPositionY )
			{
				MaxPositionY = CurrentSprite->GetPosition().y;
				CurrentSprite++;
			}
			else
			{
				OtherSprite = CurrentSprite;
				CurrentPositionY = CurrentSprite->GetPosition().y;

				while ( --OtherSprite )
				{
					if ( OtherSprite->GetPosition().y <= CurrentPositionY )
						break;
				}

				CurrentSprite = MoveAfterSprite( CurrentSprite, OtherSprite );
			}
		}
	}
}

void World::Collisions()
{
	Sprite CurrentSprite = FirstSprite();
	Sprite OtherSprite;

	float SearchRadius;
	float SearchPositionY;

	while ( CurrentSprite )
	{
		OtherSprite = CurrentSprite;
		SearchRadius = CurrentSprite->GetRadius() + MaxRadius;
		SearchPositionY = CurrentSprite->GetPosition().y;

		while ( ++OtherSprite )
		{
			if ( ( OtherSprite->GetPosition().y - SearchPositionY ) > SearchRadius )
				break;

			SpriteCollision( *CurrentSprite, *OtherSprite );
		}
	
		CurrentSprite++;
	}
}

void World::Actions()
{
	Action CurrentAction = FirstAction();

	while ( CurrentAction )
	{
		CurrentAction->OnTick();

		CurrentAction++;
	}
}

void World::Move()
{
	Sprite CurrentSprite = FirstSprite();

	while ( CurrentSprite )
	{
		CurrentSprite->Tick();
		CurrentSprite++;
	}
}

void World::RemoveDead()
{
	Sprite CurrentSprite = FirstSprite();

	while ( CurrentSprite )
	{
		if ( CurrentSprite->GetState() == Dead )
		{
			RemoveSprite( CurrentSprite );
		}
		else
		{
			CurrentSprite++;
		}
	}
}

void World::Draw()
{
	GLint Viewport[4];
	GLint Scissor[4];
	GLdouble ModelMatrix[16];
	GLdouble ProjectionMatrix[16];

	glGetIntegerv( GL_VIEWPORT, Viewport );
	glGetIntegerv( GL_SCISSOR_BOX, Scissor );
	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, ProjectionMatrix );

	vector2d Corners[5];
	vector2d Edges[5];

	Corners[0] = vector2d( float( Scissor[0]              ),
						   float( Scissor[1]              ) );
	
	Corners[1] = vector2d( float( Scissor[0] + Scissor[2] ),
						   float( Scissor[1]              ) );
	
	Corners[2] = vector2d( float( Scissor[0] + Scissor[2] ),
						   float( Scissor[1] + Scissor[3] ) );
	
	Corners[3] = vector2d( float( Scissor[0]              ),
						   float( Scissor[1] + Scissor[3] ) );

	int index;
	GLdouble TempX, TempY, TempZ;
	
	for ( index = 0; index < 4; index++ )
	{
		gluUnProject( Corners[index].x, Corners[index].y, 0.0,
					  ModelMatrix, ProjectionMatrix, Viewport,
					  &TempX, &TempY, &TempZ );

		Corners[index].x = float( TempX );
		Corners[index].y = float( TempY );
	}

	Corners[4] = Corners[0];
	
	for ( index = 0; index < 4; index++ )
		Edges[index] = Corners[index+1] - Corners[index];

	Edges[4] = Edges[0];
		
	for ( index = 1; index < 5; index++ )
		Corners[index] += Scale( Edges[index-1], MaxRadius ) - Scale( Edges[index], MaxRadius ); 
	
	Corners[0] = Corners[4];

	glEnable( GL_TEXTURE_2D );

	// Draw Sprites
	Sprite CurrentSprite = FirstSprite();
	while ( CurrentSprite )
	{
		if ( ( CrossP( Edges[0], CurrentSprite->GetPosition() - Corners[0] ) > 0 )
		  && ( CrossP( Edges[2], CurrentSprite->GetPosition() - Corners[2] ) > 0 )
		  && ( CrossP( Edges[1], CurrentSprite->GetPosition() - Corners[1] ) > 0 )
		  && ( CrossP( Edges[3], CurrentSprite->GetPosition() - Corners[3] ) > 0 ) )
		{
			CurrentSprite->Draw();
		}

		CurrentSprite++;
	}

	glDisable( GL_TEXTURE_2D );
}

void World::DrawMini()
{
	Sprite CameraSprite;
	CameraSprite = GetSpriteType( 1 ).FirstSprite();
	if ( !CameraSprite ) return;

	glPushMatrix();
	glLoadIdentity();

	glTranslatef( 325.0f, 325.0f, 0.0f );
	
	glColor4f( 0.0f, 0.0f, 0.0f, 0.5f );
	gluQuadricDrawStyle( QuadricObject, GLU_FILL );
	gluDisk( QuadricObject, 0.0f, 64.0f, 32, 1 );
	
	glColor4f( 0.0f, 0.75f, 0.0f, 1.0f );
	gluQuadricDrawStyle( QuadricObject, GLU_SILHOUETTE );
	gluDisk( QuadricObject, 0.0f, 64.0f, 32, 1 );

	glScalef( 0.1f, 0.1f, 1.0f );
	glRotatef( 90.0f - Degrees( CameraSprite->GetAngle() ), 0.0f, 0.0f, 1.0f );
	glTranslatef( -CameraSprite->GetPosition().x, -CameraSprite->GetPosition().y, 0.0f );

	glEnable( GL_TEXTURE_2D );

	// Draw Sprites
	Sprite CurrentSprite = FirstSprite();
	while ( CurrentSprite )
	{
		if ( MagSquared( CurrentSprite->GetPosition() - CameraSprite->GetPosition() ) < float( 64 * 64 * 10 * 10 ) )
			CurrentSprite->Draw();

		CurrentSprite++;
	}

	glDisable( GL_TEXTURE_2D );

	glPopMatrix();
}

void World::PostDraw()
{
	Sprite CurrentSprite = FirstSprite();

	while ( CurrentSprite )
	{
		CurrentSprite->PostDraw();
		CurrentSprite++;
	}
}

void World::ShowFrameRate()
{
	static float TotalFrameTime = 0.0f;
	static float TotalNumFrames = 0.0f;
	static float FramesPerSecond = 0.0f;
	
	TotalFrameTime += FrameTime;
	TotalNumFrames += 1.0f;

	if ( TotalFrameTime >= 0.2f )
	{
		FramesPerSecond = TotalNumFrames / TotalFrameTime;
		TotalNumFrames = 0;
		TotalFrameTime = 0;
	}

	glColor3ub( 255, 255, 255 );
	glout.MoveTo( -375, 375 );
	glout << "FPS: " << FramesPerSecond << '\n';
}

void World::KeepInBox( float WorldSize )
{
	Sprite CurrentSprite = FirstSprite();

	while ( CurrentSprite )
	{
		vector2d Position = CurrentSprite->GetPosition();
		vector2d Velocity = CurrentSprite->GetVelocity();

		if ( ( Position.x - CurrentSprite->GetRadius() ) < -WorldSize && Velocity.x < 0 )
			CurrentSprite->SetVelocity( vector2d( -Velocity.x,  Velocity.y ), -CurrentSprite->GetAngularVelocity() );
		
		if ( ( Position.x + CurrentSprite->GetRadius() ) >  WorldSize && Velocity.x > 0 )
			CurrentSprite->SetVelocity( vector2d( -Velocity.x,  Velocity.y ), -CurrentSprite->GetAngularVelocity() );

		if ( ( Position.y - CurrentSprite->GetRadius() ) < -WorldSize && Velocity.y < 0 )
			CurrentSprite->SetVelocity( vector2d(  Velocity.x, -Velocity.y ), -CurrentSprite->GetAngularVelocity() );

		if ( ( Position.y + CurrentSprite->GetRadius() ) >  WorldSize && Velocity.y > 0 )
			CurrentSprite->SetVelocity( vector2d(  Velocity.x, -Velocity.y ), -CurrentSprite->GetAngularVelocity() );

		CurrentSprite++;
	}
}

void World::DrawBox( float WorldSize )
{
	glColor3f( 1.0f, 1.0f, 1.0f );
	
	glBegin( GL_LINE_LOOP );
		glVertex2f( -WorldSize, -WorldSize );
		glVertex2f( -WorldSize,  WorldSize );
		glVertex2f(  WorldSize,  WorldSize );
		glVertex2f(  WorldSize, -WorldSize );
	glEnd();
}

void World::Tick()
{
	static timer SortTimer;
	static timer CollisionTimer;
	static timer ActionTimer;
	static timer MoveTimer;
	static timer DrawTimer;
	static timer PostDrawTimer;
	double TotalTime = 0;

	SortTimer.Start();
		SortSprites();
	SortTimer.Stop();
	TotalTime += SortTimer.GetSeconds();

	CollisionTimer.Start();
		Collisions();
		KeepInBox( 1000.0f );
	CollisionTimer.Stop();
	TotalTime += CollisionTimer.GetSeconds();
	
	ActionTimer.Start();
		Actions();
	ActionTimer.Stop();
	TotalTime += ActionTimer.GetSeconds();
	
	CalcFrameTime();
	if ( FrameTime > 0.5f ) FrameTime = 0.5f;

	glPushMatrix();
		
		MoveTimer.Start();
			Move();
			ParticleSystem.Move();
			RemoveDead();
		MoveTimer.Stop();
		TotalTime += MoveTimer.GetSeconds();

		DrawTimer.Start();
			ParticleSystem.Draw();
			Draw();
			DrawBox( 1000.0f );
			DrawMini();
		DrawTimer.Stop();
		TotalTime += DrawTimer.GetSeconds();
	
	glPopMatrix();
	
	PostDrawTimer.Start();
		PostDraw();
	PostDrawTimer.Stop();
	TotalTime += PostDrawTimer.GetSeconds();

	ShowFrameRate();

	glout << "Sort: " << SortTimer.GetSeconds() * 1000 << " mSec ( " << SortTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "Collisions: " << CollisionTimer.GetSeconds() * 1000 << " mSec ( " << CollisionTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "Actions: " << ActionTimer.GetSeconds() * 1000 << " mSec ( " << ActionTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "Move: " << MoveTimer.GetSeconds() * 1000 << " mSec ( " << MoveTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "Draw: " << DrawTimer.GetSeconds() * 1000 << " mSec ( " << DrawTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "PostDraw: " << PostDrawTimer.GetSeconds() * 1000 << " mSec ( " << PostDrawTimer.GetSeconds() / TotalTime * 100 << "% )\n";
	glout << "Frame: " << FrameTime * 1000 << " mSec\n";
	glout << "Objects: " << NumSprites();
}
