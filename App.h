#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>

class Application
{
	SDL_Window		*Window;

public:
	
	Application();
	~Application();
	
	SDL_Window *GetWindow();

	bool Init( const char *name, int width, int height, bool fullScreen );
	void Destroy();

	void SwapBuffers();
};

extern Application Program;

#endif
