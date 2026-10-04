#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <string.h>

#include "DirectInput.h"
#include "objects.h"

static bool Running = true;

void HandleEvent( const SDL_Event & event )
{
	switch ( event.type )
	{
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			DInput.Restore();
			break;

		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			gl.Resize( event.window.data1, event.window.data2 );
			break;

		case SDL_EVENT_QUIT:
			Running = false;
			break;

		default:
			break;
	}
}

void RenderFrame()
{
	glClear( GL_COLOR_BUFFER_BIT );

	DInput.Poll();
	GameWorld.Tick();

	Program.SwapBuffers();

	if ( DInput.KeyDown( DIK_LALT ) || DInput.KeyDown( DIK_RALT ) )
	{
		if ( DInput.KeyPress( DIK_Q ) ) Running = false;
	}
}

int main( int argc, char *argv[] )
{
	bool FullScreen = false;

	for ( int index = 1; index < argc; index++ )
	{
		if ( strcmp( argv[ index ], "--fullscreen" ) == 0 ) FullScreen = true;
	}

	if ( ! Program.Init( "Asteroids", 1024, 768, FullScreen ) )
		return 1;

	if ( ! gl.Init() )
		return 1;

	if ( ! DSound.Init() )
		return 1;

	if ( ! DInput.Init( Program.GetWindow() ) )
		return 1;
	
	if ( ! ParticleSystem.Init() )
		return 1;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -400, 400, -200, 400, -100, 100 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );

	glDisable( GL_DITHER );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glLineWidth( 3.0f );

	Sprite CameraSprite = GameWorld.AddSprite( new Camera(), CameraID );
	CameraSprite->SetPosition( vector2d( -32, -32 ), 0 );

	Sprite PlayerSprite = GameWorld.AddSprite( new PlayerShip(), PlayerShipID );
	PlayerSprite->SetPosition( vector2d( 32, 32 ), 0 );
	
	GameWorld.AddAction( CameraSprite, PlayerSprite, new CameraLock() ); 

	for ( int index = 0; index < 200; index++ )
	{
		Sprite NewSprite = GameWorld.AddSprite( new Asteroid(rand()%4), AsteroidID );

		NewSprite->SetPosition( vector2d( float( rand()%2001 - 1000 ), float( rand()%2001 - 1000 ) ),
							    Radians( float( rand()%360 ) ) );
	}
	
	while ( Running )
	{
		SDL_Event event;

		while ( SDL_PollEvent( &event ) ) HandleEvent( event );

		RenderFrame();
	}

	gl.Destroy();
	Program.Destroy();

	return 0;
}
