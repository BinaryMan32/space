#include "DirectInput.h"
#include "objects.h"

long CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		case WM_ACTIVATE:
			DInput.Restore();
			break;
	
		case WM_PAINT:
			break;

		case WM_SIZE:
			if ( wParam != SIZE_MINIMIZED )
				gl.Resize( lParam & 0x0000FFFF, lParam >> 16 );
			break;

		case WM_DESTROY:
			gl.Destroy();
			Program.RestoreDisplayMode();
			PostQuitMessage( 0 );
			break;

		default:
			break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

void RenderFrame()
{
	glClear( GL_COLOR_BUFFER_BIT );

	DInput.Poll();
	GameWorld.Tick();

	SwapBuffers( Program.GetHDC() );

	if ( DInput.KeyDown( DIK_LALT ) || DInput.KeyDown( DIK_RALT ) )
	{
		if ( DInput.KeyPress( DIK_Q ) ) PostMessage( Program.GetHWND(), WM_CLOSE, 0, 0 );
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR LpCmdLine, int ShowCmd )
{
	MSG msg;

	Program.SaveDisplayMode();
	Program.SetDisplayMode( 1024, 768, 32 );

//	if ( ! Program.Init( hInstance, ShowCmd, "Asteroids", 800, 600 ) )
	if ( ! Program.InitFullScreen( hInstance, ShowCmd, "Asteroids" ) )
		return false;

	gl.Init();

	if ( ! DSound.Init( Program.GetHWND() ) )
		return false;

	if ( ! DInput.Init( Program.GetHINSTANCE(), Program.GetHWND() ) )
		return false;
	
	if ( ! ParticleSystem.Init() )
		return false;

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
	
	while ( true )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( ! GetMessage( &msg, NULL, 0, 0 ) ) return msg.wParam;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			RenderFrame();
		}
	}

	return 1;
}
