#ifndef SOUND_H
#define SOUND_H

#include <SDL3/SDL.h>

#include "gamestring.h"

class Sound;
class SoundSystem;

extern SoundSystem Audio; // SDL audio device and loaded sounds

class SoundSystem
{
	private:

	struct SoundBuffer
	{
		string Name;
		SDL_AudioSpec Spec;
		Uint8 *Data;
		Uint32 Length;
	};
	
	SDL_AudioDeviceID Device;

	SoundBuffer *Buffers;
	int MaxBuffers;
	int NumBuffers;
	
	// streams bound to the device, which mixes everything they play
	SDL_AudioStream **Voices;
	int NumVoices;
	
	bool PlayBuffer( int index );
	int LoadBuffer( const char *FileName );

	public:
	
	SoundSystem();
	~SoundSystem();
	
	bool Init( int MaxSoundBuffers = 128, int MaxVoices = 32 );
	void Destroy();

	friend class Sound;
};

class Sound
{
	int BufferID;

	public:

	Sound()
	{
		BufferID = -1;
	}

	Sound( const char *FileName )
	{
		BufferID = Audio.LoadBuffer( FileName );
	}

	bool Load( const char *FileName )
	{
		BufferID = Audio.LoadBuffer( FileName );
		return ( BufferID >= 0 );
	}

	bool Play()
	{
		return Audio.PlayBuffer( BufferID );
	}

	bool Valid()
	{
		return ( BufferID >= 0 );
	}
};

#endif
