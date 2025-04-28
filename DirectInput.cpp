#include "DirectInput.h"

DirectInputClass DInput;

DirectInputClass::DirectInputClass()
{
	lpDirectInput = NULL;
	lpMouse = NULL;
	lpKeyboard = NULL;
	
	ZeroMemory( &KeyState, sizeof( KeyState ) );

	width = 0;
	height = 0;
}

DirectInputClass::~DirectInputClass()
{
	lpMouse->Unacquire();
	lpMouse->Release();

	lpKeyboard->Unacquire();
	lpKeyboard->Release();
	
	lpDirectInput->Release();
}

void DirectInputClass::SetCursorPosition( int posX, int posY )
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

bool DirectInputClass::SetDisplaySize( int theWidth, int theHeight )
{
	if ( theWidth <= 0 || theHeight <= 0 ) return false;
	
	width = theWidth;
	height = theHeight;

	x = width / 2;
	y = height / 2;

	return true;
}

bool DirectInputClass::Init( HINSTANCE InstanceHandle, HWND WindowHandle )
{
	// Get DirectInput Interface
	hr = DirectInput8Create(	InstanceHandle, 
								DIRECTINPUT_VERSION,
								IID_IDirectInput8A,
								(void **)&lpDirectInput,
								NULL );
	
	if ( hr != DI_OK ) return false;
	
	// Get Mouse Device Interface
	hr = lpDirectInput->CreateDevice(	GUID_SysMouse,
										&lpMouse,
										NULL );

	if ( hr != DI_OK ) return false;

	// Get Keyboard Device Interface
	hr = lpDirectInput->CreateDevice(	GUID_SysKeyboard,
										&lpKeyboard,
										NULL );

	if ( hr != DI_OK ) return false;

	lpMouse->SetDataFormat( &c_dfDIMouse );
	lpKeyboard->SetDataFormat( &c_dfDIKeyboard );
	
	lpMouse->SetCooperativeLevel( WindowHandle, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	lpKeyboard->SetCooperativeLevel( WindowHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	lpMouse->Acquire();
	lpKeyboard->Acquire();

	x = width / 2;
	y = height / 2;

	return true;
}

bool DirectInputClass::Restore()
{
	if ( lpDirectInput == NULL ) return false;

	lpMouse->Acquire();
	lpKeyboard->Acquire();

	return true;
}

void DirectInputClass::Poll()
{
	int i = 260;

	while ( i-- ) OldKeyState[ i ] = KeyState[ i ];

	lpKeyboard->GetDeviceState( 256, KeyState );
	
	lpMouse->GetDeviceState( sizeof(MouseState), &MouseState );

	KeyState[ DIK_LEFTMOUSE ]   = MouseState.rgbButtons[ 0 ]; 
	KeyState[ DIK_RIGHTMOUSE ]  = MouseState.rgbButtons[ 1 ]; 
	KeyState[ DIK_MIDDLEMOUSE ] = MouseState.rgbButtons[ 2 ]; 
	KeyState[ DIK_THUMBMOUSE ]  = MouseState.rgbButtons[ 3 ]; 
	
	dx = MouseState.lX;
	dy = MouseState.lY;
	
	x += dx;
	y += dy;
	
	if ( x < 0 ) x = 0;
	if ( x >= width ) x = width - 1;
	if ( y < 0 ) y = 0;
	if ( y >= height ) y = height - 1;
}

bool DirectInputClass::KeyDown( int index )
{
	if ( index < 0 || index >= 260 ) return false;
	if ( KeyState[ index ] & 0x80 ) return true;
	
	return false;
}

bool DirectInputClass::KeyPress( int index )
{
	if ( index < 0 || index >= 260 ) return false;
	if ( ( KeyState[ index ] & 0x80 ) && !( OldKeyState[ index ] & 0x80 ) ) return true;
	
	return false;
}

bool DirectInputClass::KeyRelease( int index )
{
	if ( index < 0 || index >= 260 ) return false;
	if ( !( KeyState[ index ] & 0x80 ) && ( OldKeyState[ index ] & 0x80 ) ) return true;
	
	return false;
}
