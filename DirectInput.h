#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#define DIK_LEFTMOUSE	256
#define DIK_RIGHTMOUSE	257
#define DIK_MIDDLEMOUSE	258
#define DIK_THUMBMOUSE	259

class DirectInputClass;

extern DirectInputClass DInput;

class DirectInputClass
{
	LPDIRECTINPUT8 lpDirectInput;
	LPDIRECTINPUTDEVICE8 lpMouse;
	LPDIRECTINPUTDEVICE8 lpKeyboard;
	DIMOUSESTATE MouseState;
	char KeyState[ 260 ];
	char OldKeyState[ 260 ];
	HRESULT hr;

	public:
	
	int width, height;
	int x,y,dx,dy;
	
	DirectInputClass();
	~DirectInputClass();
	
	void SetCursorPosition( int posX, int posY );
	bool SetDisplaySize( int theWidth, int theHeight );

	bool Init( HINSTANCE InstanceHandle, HWND WindowHandle );
	bool Restore();

	void Poll();

	bool KeyDown( int index );
	bool KeyPress( int index );
	bool KeyRelease( int index );
};

#endif
