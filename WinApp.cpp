#include "WinApp.h"

WindowsApp Program;

WindowsApp::WindowsApp()
{
	InstanceHandle = NULL;
	WindowHandle = NULL;
	DeviceContextHandle = NULL;
}

WindowsApp::~WindowsApp()
{
	if ( DeviceContextHandle != NULL ) ReleaseDC( WindowHandle, DeviceContextHandle );
}

HINSTANCE WindowsApp::GetHINSTANCE()
{
	return InstanceHandle;
}

HWND WindowsApp::GetHWND()
{
	return WindowHandle;
}

HDC WindowsApp::GetHDC()
{
	return DeviceContextHandle;
}

bool WindowsApp::SaveDisplayMode()
{
	return ( SaveDC( DeviceContextHandle ) != 0 );
}

bool WindowsApp::RestoreDisplayMode()
{
	return ( RestoreDC( DeviceContextHandle, -1 ) != 0 );
}

bool WindowsApp::SetDisplayMode( int width, int height, int bpp )
{
	DEVMODE DisplayMode;

	DisplayMode.dmSize = sizeof( DisplayMode );
	DisplayMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	DisplayMode.dmPelsWidth = width;
	DisplayMode.dmPelsHeight = height;
	DisplayMode.dmBitsPerPel = bpp;

	return ( ChangeDisplaySettings( &DisplayMode, CDS_FULLSCREEN ) == DISP_CHANGE_SUCCESSFUL );
}

bool WindowsApp::Init( HANDLE hInstance, int nCmdShow, char *name, int width, int height )
{
	WNDCLASS wc;
	InstanceHandle = (HINSTANCE) hInstance;

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = InstanceHandle;
	wc.hIcon = LoadIcon( InstanceHandle, MAKEINTATOM( 0x1100 ) );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name;

	if ( !RegisterClass( &wc ) ) return false;

	WindowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		name,
		name,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		( GetSystemMetrics(SM_CXSCREEN) - width  ) / 2,
		( GetSystemMetrics(SM_CYSCREEN) - height ) / 2,
		width,
		height,
		NULL,
		NULL,
		InstanceHandle,
		NULL);

	if ( WindowHandle == NULL ) return false;

	DeviceContextHandle = GetDC( WindowHandle );

	if ( DeviceContextHandle == NULL ) return false;
	
	return true;
}

bool WindowsApp::InitFullScreen( HANDLE hInstance, int nCmdShow, char *name )
{
	WNDCLASS wc;
	InstanceHandle = (HINSTANCE) hInstance;

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = InstanceHandle;
	wc.hIcon = LoadIcon( InstanceHandle, MAKEINTATOM( 0x1100 ) );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name;

	if ( !RegisterClass( &wc ) ) return false;

	WindowHandle = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_APPWINDOW,
		name,
		name,
		WS_POPUP | WS_VISIBLE,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		InstanceHandle,
		NULL);

	if ( WindowHandle == NULL ) return false;

	DeviceContextHandle = GetDC( WindowHandle );

	if ( DeviceContextHandle == NULL ) return false;
	
	return true;
}
