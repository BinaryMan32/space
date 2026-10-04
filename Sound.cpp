#include "Sound.h"

SoundSystem Audio; // SDL audio device and loaded sounds

/////////////////////////////////////////////////////////////////////////////////////////
//	SoundSystem Class definitions	/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

bool SoundSystem::PlayBuffer( int BufferID )
{
	if ( ( BufferID < 0 ) || ( BufferID >= NumBuffers ) ) return false;

	SoundBuffer & Buffer = Buffers[ BufferID ];

	// play the sound on the first idle voice, if every voice is busy the sound is skipped
	for ( int index = 0; index < NumVoices; index++ )
	{
		if ( SDL_GetAudioStreamQueued( Voices[ index ] ) > 0 ) continue;

		if ( ! SDL_SetAudioStreamFormat( Voices[ index ], &Buffer.Spec, NULL ) ||
			 ! SDL_PutAudioStreamData( Voices[ index ], Buffer.Data, Buffer.Length ) )
		{
			SDL_Log( "[ Sound.Play() ] - %s", SDL_GetError() );
			return false;
		}

		return true;
	}

	return false;
}

int SoundSystem::LoadBuffer( const char *FileName )
{
	if ( NumBuffers >= MaxBuffers ) return -1;
	
	// If sound is already loaded, point to previously loaded buffer
	for ( int i=0; i<NumBuffers; i++ )
	{
		if ( Buffers[i].Name == FileName ) return i;
	}

	SoundBuffer & Buffer = Buffers[ NumBuffers ];

	if ( ! SDL_LoadWAV( FileName, &Buffer.Spec, &Buffer.Data, &Buffer.Length ) )
	{
		SDL_Log( "[ Sound.Load() ] - %s: %s", FileName, SDL_GetError() );
		return -1;
	}

	// record the file name of the sound
	Buffer.Name = FileName;

	return NumBuffers++;
}

SoundSystem::SoundSystem()
{
	Device = 0;

	Buffers = NULL;
	MaxBuffers = 0;
	NumBuffers = 0;

	Voices = NULL;
	NumVoices = 0;
}

SoundSystem::~SoundSystem()
{
	Destroy();
}

bool SoundSystem::Init( int MaxSoundBuffers, int MaxVoices )
{
	if ( Device != 0 ) return false;
	
	Device = SDL_OpenAudioDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL );

	if ( Device == 0 )
	{
		SDL_Log( "[ Audio.Init() ] - SDL_OpenAudioDevice() Failed: %s", SDL_GetError() );
		return false;
	}

	MaxBuffers = MaxSoundBuffers;
	NumBuffers = 0;
	Buffers = new SoundBuffer [ MaxBuffers ];

	Voices = new SDL_AudioStream * [ MaxVoices ];

	// the source format is set per sound when it is played
	SDL_AudioSpec VoiceSpec = { SDL_AUDIO_S16, 2, 44100 };

	for ( NumVoices = 0; NumVoices < MaxVoices; NumVoices++ )
	{
		SDL_AudioStream *Voice = SDL_CreateAudioStream( &VoiceSpec, NULL );

		if ( Voice == NULL ) break;

		if ( ! SDL_BindAudioStream( Device, Voice ) )
		{
			SDL_DestroyAudioStream( Voice );
			break;
		}

		Voices[ NumVoices ] = Voice;
	}

	if ( NumVoices == 0 )
	{
		SDL_Log( "[ Audio.Init() ] - SDL_CreateAudioStream() Failed: %s", SDL_GetError() );
		return false;
	}
	
	return true;
}

void SoundSystem::Destroy()
{
	while ( NumVoices > 0 )
	{
		SDL_DestroyAudioStream( Voices[ --NumVoices ] );
	}
	
	delete [] Voices;
	Voices = NULL;

	while ( NumBuffers > 0 )
	{
		SDL_free( Buffers[ --NumBuffers ].Data );
	}
	
	delete [] Buffers;
	Buffers = NULL;

	if ( Device != 0 )
	{
		SDL_CloseAudioDevice( Device );
		Device = 0;
	}
}
