#include "App.h"

Application Program;

Application::Application()
{
	Window = NULL;
}

Application::~Application()
{
	Destroy();
}

SDL_Window *Application::GetWindow()
{
	return Window;
}

bool Application::Init( const char *name, int width, int height, bool fullScreen )
{
	if ( ! SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) )
	{
		SDL_Log( "SDL_Init() failed: %s", SDL_GetError() );
		return false;
	}

	// double buffered RGBA with an 8-bit alpha channel, using the legacy fixed-function pipeline
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if ( fullScreen ) flags |= SDL_WINDOW_FULLSCREEN;

	Window = SDL_CreateWindow( name, width, height, flags );

	if ( Window == NULL )
	{
		SDL_Log( "SDL_CreateWindow() failed: %s", SDL_GetError() );
		return false;
	}

	return true;
}

void Application::Destroy()
{
	if ( Window != NULL )
	{
		SDL_DestroyWindow( Window );
		Window = NULL;
		SDL_Quit();
	}
}

void Application::SwapBuffers()
{
	SDL_GL_SwapWindow( Window );
}
