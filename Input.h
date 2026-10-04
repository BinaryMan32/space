#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

// keys are indexed by SDL scancode, mouse buttons follow the keyboard scancodes
#define KEY_LEFTMOUSE	( SDL_SCANCODE_COUNT + 0 )
#define KEY_RIGHTMOUSE	( SDL_SCANCODE_COUNT + 1 )
#define KEY_MIDDLEMOUSE	( SDL_SCANCODE_COUNT + 2 )
#define KEY_THUMBMOUSE	( SDL_SCANCODE_COUNT + 3 )
#define NUM_KEYS		( SDL_SCANCODE_COUNT + 4 )

class InputClass;

extern InputClass Input;

class InputClass
{
	SDL_Window *Window;
	bool KeyState[ NUM_KEYS ];
	bool OldKeyState[ NUM_KEYS ];
	float RemainderX, RemainderY;

	public:
	
	int width, height;
	int x,y,dx,dy;
	
	InputClass();
	
	void SetCursorPosition( int posX, int posY );
	bool SetDisplaySize( int theWidth, int theHeight );

	bool Init( SDL_Window *theWindow );
	bool Restore();

	void Poll();

	bool KeyDown( int index );
	bool KeyPress( int index );
	bool KeyRelease( int index );
};

#endif
