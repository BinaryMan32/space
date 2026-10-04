#include "Input.h"

InputClass Input;

InputClass::InputClass()
{
	Window = NULL;
	
	SDL_zeroa( KeyState );
	SDL_zeroa( OldKeyState );

	RemainderX = RemainderY = 0.0f;

	width = 0;
	height = 0;

	x = y = dx = dy = 0;
}

void InputClass::SetCursorPosition( int posX, int posY )
{
	dx = 0;
	dy = 0;

	x = posX;
	y = posY;

	if ( x < 0 ) x = 0;
	if ( x >= width ) x = width - 1;
	if ( y < 0 ) y = 0;
	if ( y >= height ) y = height - 1;
}

bool InputClass::SetDisplaySize( int theWidth, int theHeight )
{
	if ( theWidth <= 0 || theHeight <= 0 ) return false;
	
	width = theWidth;
	height = theHeight;

	x = width / 2;
	y = height / 2;

	return true;
}

bool InputClass::Init( SDL_Window *theWindow )
{
	Window = theWindow;

	// hide the cursor and report relative mouse motion, like an exclusive DirectInput mouse
	if ( ! Restore() ) return false;

	// discard any motion that happened before the game started
	SDL_GetRelativeMouseState( NULL, NULL );

	x = width / 2;
	y = height / 2;

	return true;
}

bool InputClass::Restore()
{
	if ( Window == NULL ) return false;

	if ( ! SDL_SetWindowRelativeMouseMode( Window, true ) )
	{
		SDL_Log( "SDL_SetWindowRelativeMouseMode() failed: %s", SDL_GetError() );
		return false;
	}

	return true;
}

void InputClass::Poll()
{
	SDL_memcpy( OldKeyState, KeyState, sizeof( KeyState ) );

	int NumKeys = 0;
	const bool *Keyboard = SDL_GetKeyboardState( &NumKeys );

	if ( NumKeys > SDL_SCANCODE_COUNT ) NumKeys = SDL_SCANCODE_COUNT;
	SDL_memcpy( KeyState, Keyboard, NumKeys * sizeof( bool ) );

	float MouseX = 0.0f, MouseY = 0.0f;
	SDL_MouseButtonFlags Buttons = SDL_GetRelativeMouseState( &MouseX, &MouseY );

	KeyState[ KEY_LEFTMOUSE ]   = ( Buttons & SDL_BUTTON_LMASK ) != 0;
	KeyState[ KEY_RIGHTMOUSE ]  = ( Buttons & SDL_BUTTON_RMASK ) != 0;
	KeyState[ KEY_MIDDLEMOUSE ] = ( Buttons & SDL_BUTTON_MMASK ) != 0;
	KeyState[ KEY_THUMBMOUSE ]  = ( Buttons & SDL_BUTTON_X1MASK ) != 0;

	// SDL reports fractional motion, carry the fractions over to the next frame
	RemainderX += MouseX;
	RemainderY += MouseY;

	dx = int( RemainderX );
	dy = int( RemainderY );

	RemainderX -= dx;
	RemainderY -= dy;
	
	x += dx;
	y += dy;
	
	if ( x < 0 ) x = 0;
	if ( x >= width ) x = width - 1;
	if ( y < 0 ) y = 0;
	if ( y >= height ) y = height - 1;
}

bool InputClass::KeyDown( int index )
{
	if ( index < 0 || index >= NUM_KEYS ) return false;
	
	return KeyState[ index ];
}

bool InputClass::KeyPress( int index )
{
	if ( index < 0 || index >= NUM_KEYS ) return false;
	
	return KeyState[ index ] && !OldKeyState[ index ];
}

bool InputClass::KeyRelease( int index )
{
	if ( index < 0 || index >= NUM_KEYS ) return false;
	
	return !KeyState[ index ] && OldKeyState[ index ];
}
