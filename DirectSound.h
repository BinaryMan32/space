#ifndef DIRECTSOUND_H
#define DIRECTSOUND_H

#include <dsound.h>
#include "string.h"

class Sound;
class DirectSoundClass;

extern DirectSoundClass DSound; // DirectSound Interface

#define NO_STRUCT_PADDING	1
#pragma pack( push, NO_STRUCT_PADDING )

struct RiffHeaderType
{
	char ID[4];
	unsigned int ChunkLength;
};

struct WaveHeaderType
{
	char ID[4];

	struct WaveFormatType
	{
		char ID[4];
		unsigned int   Length;
		unsigned short wFormatTag;
		unsigned short nChannels;
		unsigned int   nSamplesPerSec;
		unsigned int   nAvgBytesPerSec;
		unsigned short nBlockAlign;
		unsigned short wBitsPerSample;
	} Format;
	
	struct WaveDataType
	{
		char ID[4];
		unsigned int Length;
	} Data;
};

#pragma pack( pop, NO_STRUCT_PADDING )
#undef NO_STRUCT_PADDING

class DirectSoundClass
{
	private:
	
	LPDIRECTSOUND lpDirectSound;
	DSCAPS Caps;
	HRESULT hr;
	HWND WindowHandle;

	LPDIRECTSOUNDBUFFER lpPrimaryBuffer;
	
	LPDIRECTSOUNDBUFFER *lpSoundBuffer;
	string *SoundNames;
	int MaxBuffers;
	int NumBuffers;
	
	LPDIRECTSOUNDBUFFER *lpDupSoundBuffer;
	int MaxDupBuffers;
	int NumDupBuffers;
	
	bool CheckForError( string ErrorMessage );
	bool PlayBuffer( int index );
	int LoadBuffer( char *FileName );
	int LoadWaveFile( char *FileName );

	public:
	
	DirectSoundClass();
	~DirectSoundClass();
	
	bool Init( HWND theWindowHandle, int MaxSoundBuffers = 128 );

	friend Sound;
};

class Sound
{
	int BufferID;

	public:

	Sound()
	{
		BufferID = -1;
	}

	Sound( char *FileName )
	{
		BufferID = DSound.LoadBuffer( FileName );
	}

	bool Load( char *FileName )
	{
		BufferID = DSound.LoadBuffer( FileName );
		return ( BufferID >= 0 );
	}

	bool Play()
	{
		return DSound.PlayBuffer( BufferID );
	}

	bool Valid()
	{
		return ( BufferID >= 0 );
	}
};

#endif
