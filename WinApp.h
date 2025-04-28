#ifndef WINAPP_H
#define WINAPP_H

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

long CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

class WindowsApp
{
	HINSTANCE		InstanceHandle;
	HWND			WindowHandle;
	HDC				DeviceContextHandle;

public:
	
	WindowsApp();
	~WindowsApp();
	
	HINSTANCE GetHINSTANCE();
	HWND GetHWND();
	HDC GetHDC();

	bool Init( HANDLE hInstance, int nCmdShow, char *name, int width, int height );
	bool InitFullScreen( HANDLE hInstance, int nCmdShow, char *name );

	bool SaveDisplayMode();
	bool RestoreDisplayMode();
	bool SetDisplayMode( int width, int height, int bpp );
};

extern WindowsApp Program;

#endif
