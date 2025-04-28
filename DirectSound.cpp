#include <stdio.h>

#include "queue.h"

#include "DirectSound.h"

DirectSoundClass DSound; // DirectSound Interface

/////////////////////////////////////////////////////////////////////////////////////////
//	DirectSound Class definitions	/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

bool DirectSoundClass::CheckForError( string ErrorMessage )
{
	if ( hr == DS_OK ) return false;
	OutputDebugString( ErrorMessage );
		
	switch ( hr )
	{
		case DSERR_GENERIC:				MessageBox( WindowHandle, "An undetermined error occurred inside the DirectSound subsystem.", ErrorMessage, MB_OK );
			break;
		case DSERR_ACCESSDENIED:		MessageBox( WindowHandle, "The request failed because access was denied.", ErrorMessage, MB_OK );
			break;
		case DSERR_ALLOCATED:			MessageBox( WindowHandle, "The request failed because resources, such as a priority level, were already in use by another caller.", ErrorMessage, MB_OK );
			break;
		case DSERR_ALREADYINITIALIZED:	MessageBox( WindowHandle, "The object is already initialized.", ErrorMessage, MB_OK );
			break;
		case DSERR_BADFORMAT:			MessageBox( WindowHandle, "The specified wave format is not supported.", ErrorMessage, MB_OK );
			break;
		case DSERR_BUFFERLOST:			MessageBox( WindowHandle, "The buffer memory has been lost and must be restored.", ErrorMessage, MB_OK );
			break;
		case DSERR_CONTROLUNAVAIL:		MessageBox( WindowHandle, "The buffer control (volume, pan, and so on) requested by the caller is not available.", ErrorMessage, MB_OK );
			break;
		case DSERR_INVALIDCALL:			MessageBox( WindowHandle, "This function is not valid for the current state of this object.", ErrorMessage, MB_OK );
			break;
		case DSERR_INVALIDPARAM:		MessageBox( WindowHandle, "An invalid parameter was passed to the returning function.", ErrorMessage, MB_OK );
			break;
		case DSERR_NOAGGREGATION:		MessageBox( WindowHandle, "The object does not support aggregation.", ErrorMessage, MB_OK );
			break;
		case DSERR_NODRIVER:			MessageBox( WindowHandle, "No sound driver is available for use.", ErrorMessage, MB_OK );
			break;
		case DSERR_NOINTERFACE:			MessageBox( WindowHandle, "The requested COM interface is not available.", ErrorMessage, MB_OK );
			break;
		case DSERR_OTHERAPPHASPRIO:		MessageBox( WindowHandle, "Another application has a higher priority level, preventing this call from succeeding.", ErrorMessage, MB_OK );
			break;
		case DSERR_OUTOFMEMORY:			MessageBox( WindowHandle, "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.", ErrorMessage, MB_OK );
			break;
		case DSERR_PRIOLEVELNEEDED:		MessageBox( WindowHandle, "The caller does not have the priority level required for the function to succeed.", ErrorMessage, MB_OK );
			break;
		case DSERR_UNINITIALIZED:		MessageBox( WindowHandle, "The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.", ErrorMessage, MB_OK );
			break;
		case DSERR_UNSUPPORTED:			MessageBox( WindowHandle, "The function called is not supported at this time.", ErrorMessage, MB_OK );
			break;
	}

	return true;
}

bool DirectSoundClass::PlayBuffer( int BufferID )
{
	if ( ( BufferID < 0 ) || ( BufferID >= NumBuffers ) ) return false;

	if ( lpSoundBuffer[ BufferID ] == NULL ) return false;
	
	DWORD SoundStatus;

	hr = lpSoundBuffer[ BufferID ]->GetStatus( &SoundStatus );
	if ( CheckForError( "[ Sound.Play() ] - GetStatus() Failed!" ) ) return false;

	// play the master sound buffer if it is not playing
	if ( ! ( SoundStatus & DSBSTATUS_PLAYING ) )
	{
		hr = lpSoundBuffer[ BufferID ]->Play( 0, 0, 0 );
		if ( CheckForError( "[ Sound.Play() ] - Play() Failed!" ) ) return false;

		return true;
	}
	else
	{
		// look for an unused duplicate buffer
		for ( int index=0; index<NumDupBuffers; index++ )
		{
			hr = lpDupSoundBuffer[ index ]->GetStatus( &SoundStatus );
			if ( CheckForError( "[ Sound.Play() ] - GetStatus() Failed!" ) ) return false;
			
			if ( ! ( SoundStatus & DSBSTATUS_PLAYING ) )
			{
				hr = lpDupSoundBuffer[ index ]->Release();
				if ( CheckForError( "[ Sound.Play() ] - Release() Failed!" ) ) return false;
				
				hr = lpDirectSound->DuplicateSoundBuffer( lpSoundBuffer[ BufferID ], &lpDupSoundBuffer[ index ] );
				if ( CheckForError( "[ Sound.Play() ] - DuplicateSoundBuffer() Failed!" ) ) return false;

				hr = lpDupSoundBuffer[ index ]->Play( 0, 0, 0 );
				if ( CheckForError( "[ Sound.Play() ] - Play() Failed!" ) ) return false;
			
				break;
			}
		}

		if ( index < NumDupBuffers )
		{
			// sound has been successfully played, free extra duplicate buffers
			for ( ; index < NumDupBuffers; index++ )
			{
				hr = lpDupSoundBuffer[ index ]->GetStatus( &SoundStatus );
				if ( CheckForError( "[ Sound.Play() ] - GetStatus() Failed!" ) ) return false;
				
				if ( ! ( SoundStatus & DSBSTATUS_PLAYING ) )
				{
					hr = lpDupSoundBuffer[ index ]->Release();
					if ( CheckForError( "[ Sound.Play() ] - Release() Failed!" ) ) return false;
				
					lpDupSoundBuffer[ index-- ] = lpDupSoundBuffer[ --NumDupBuffers ];
					lpDupSoundBuffer[ NumDupBuffers ] = NULL;
				}
			}
		}
		else
		{
			// if more duplicate buffer slots are available, create a duplicate buffer and play it
			if ( NumDupBuffers < MaxDupBuffers )
			{
				hr = lpDirectSound->DuplicateSoundBuffer( lpSoundBuffer[ BufferID ], &lpDupSoundBuffer[ NumDupBuffers ] );
				if ( CheckForError( "[ Sound.Play() ] - DuplicateSoundBuffer() Failed!" ) ) return false;
	
				hr = lpDupSoundBuffer[ NumDupBuffers ]->Play( 0, 0, 0 );
				if ( CheckForError( "[ Sound.Play() ] - Play() Failed!" ) ) return false;
				
				NumDupBuffers++;
			}
		}	
	}

	return true;
}

int DirectSoundClass::LoadBuffer( char *FileName )
{
	if ( NumBuffers >= MaxBuffers ) return -1;
	
	// If sound is already loaded, point to previously loaded buffer
	for ( int i=0; i<NumBuffers; i++ )
	{
		if ( SoundNames[i] == FileName ) return i;
	}

	return LoadWaveFile( FileName );
}

int DirectSoundClass::LoadWaveFile( char *FileName )
{
	RiffHeaderType RiffHeader;
	WaveHeaderType WaveHeader;
	unsigned long SoundLength = 0;
	void *SoundData;

	// open the sound file
	FILE *SoundFile = fopen( FileName, "rb" );

	// check if file exists
	if ( SoundFile == NULL )
	{
		OutputDebugString( "[ Sound.Load() ] - File Not Found!\n" );
		return -1;
	}

	// read RIFF header
	fread( &RiffHeader, sizeof( RiffHeader ), 1, SoundFile );
	
	// check if riff header is valid
	if ( RiffHeader.ID[0] != 'R' || RiffHeader.ID[1] != 'I' || RiffHeader.ID[2] != 'F' || RiffHeader.ID[3] != 'F' )
	{
		OutputDebugString( "[ Sound.Load() ] - Bad Riff Header!\n" );
		return -1;
	}

	// read WAVE chunk
	fread( &WaveHeader, sizeof( WaveHeader ), 1, SoundFile );
	
	// check if wave chunk is valid
	if ( WaveHeader.ID[0] != 'W' || WaveHeader.ID[1] != 'A' || WaveHeader.ID[2] != 'V' || WaveHeader.ID[3] != 'E' )
	{
		OutputDebugString( "[ Sound.Load() ] - Bad Wave Header!\n" );
		return -1;
	}
	
	// check if format header is valid
	if ( WaveHeader.Format.ID[0] != 'f' || WaveHeader.Format.ID[1] != 'm' || WaveHeader.Format.ID[2] != 't' || WaveHeader.Format.ID[3] != ' ' )
	{
		OutputDebugString( "[ Sound.Load() ] - Bad Wave Format Header!\n" );
		return -1;
	}

	// check if data header is valid
	if ( WaveHeader.Data.ID[0] != 'd' || WaveHeader.Data.ID[1] != 'a' || WaveHeader.Data.ID[2] != 't' || WaveHeader.Data.ID[3] != 'a' )
	{
		OutputDebugString( "[ Sound.Load() ] - Bad Wave Data Header!\n" );
		return -1;
	}

	// setup sound format structure
	WAVEFORMATEX SoundFormat;
    ZeroMemory( &SoundFormat, sizeof( SoundFormat ) ); 
	SoundFormat.wFormatTag = WaveHeader.Format.wFormatTag;
	SoundFormat.nSamplesPerSec = WaveHeader.Format.nSamplesPerSec;
	SoundFormat.wBitsPerSample = WaveHeader.Format.wBitsPerSample;
	SoundFormat.nChannels = WaveHeader.Format.nChannels;
	SoundFormat.nBlockAlign = WaveHeader.Format.nBlockAlign;
	SoundFormat.nAvgBytesPerSec = WaveHeader.Format.nAvgBytesPerSec;
	SoundFormat.cbSize = 0;
	
	// setup directsound buffer structure
	DSBUFFERDESC Desc;
	ZeroMemory( &Desc, sizeof( Desc ) );
	Desc.dwSize = sizeof( Desc );
	Desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
	Desc.dwBufferBytes = WaveHeader.Data.Length;
	Desc.lpwfxFormat = &SoundFormat;
	
	// create directsound buffer
	hr = DSound.lpDirectSound->CreateSoundBuffer( &Desc, &lpSoundBuffer[ NumBuffers ], NULL );
	if ( CheckForError( "[ Sound.Load() ] - CreateSoundBuffer() Failed!" ) ) return -1;
	
	// lock buffer
	hr = lpSoundBuffer[ NumBuffers ]->Lock( 0, 0, &SoundData, &SoundLength, NULL, NULL, DSBLOCK_ENTIREBUFFER );
	if ( CheckForError( "[ Sound.Load() ] - Lock() Failed!" ) ) return -1;

	// read sound data
	if ( fread( SoundData, 1, SoundLength, SoundFile ) < SoundLength )
	{
		OutputDebugString( "[ Sound.Load() ] - Error Reading Wave Data!\n" );
		lpSoundBuffer[ NumBuffers ]->Unlock( SoundData, SoundLength, NULL, 0 );
		return -1;
	}

	// unlock buffer
	hr = lpSoundBuffer[ NumBuffers ]->Unlock( SoundData, SoundLength, NULL, 0 );
	if ( CheckForError( "[ Sound.Load() ] - Unlock() Failed!" ) ) return -1;
	
	// close the sound file
	if ( fclose( SoundFile ) )
	{
		OutputDebugString( "[ Sound.Load() ] - File Not Closed!\n" );
	}
	
	// record the file name of the sound
	SoundNames[ NumBuffers ] = FileName;

	return NumBuffers++;
}

DirectSoundClass::DirectSoundClass()
{
	lpDirectSound = NULL;
	lpPrimaryBuffer = NULL;

	ZeroMemory( &Caps, sizeof( Caps ) );
	Caps.dwSize = sizeof( Caps );
}

DirectSoundClass::~DirectSoundClass()
{
	delete [] SoundNames;
	
	while ( --NumBuffers >= 0 )
	{
		hr = lpSoundBuffer[ NumBuffers ]->Release();
		CheckForError( "[ DSound Destructor ] - Release() Failed!" );
	}
	
	delete [] lpSoundBuffer;

	while ( --NumDupBuffers >= 0 )
	{
		hr = lpDupSoundBuffer[ NumDupBuffers ]->Release();
		CheckForError( "[ DSound Destructor ] - Release() Failed!" );
	}
	
	delete [] lpDupSoundBuffer;
	
	hr = lpPrimaryBuffer->Release();
	CheckForError( "[ DSound Destructor ] - Release() Failed!" );

	hr = lpDirectSound->Release();
	CheckForError( "[ DSound Destructor ] - Release() Failed!" );
}

bool DirectSoundClass::Init( HWND theWindowHandle, int MaxSoundBuffers )
{
	if ( lpDirectSound != NULL ) return false;
	
	WindowHandle = theWindowHandle;
	MaxBuffers = MaxSoundBuffers;
	
	hr = DirectSoundCreate( NULL, &lpDirectSound, NULL );
	if ( CheckForError( "[ DSound.Init() ] - DirectSoundCreate() Failed!" ) )
		return false;

	hr = lpDirectSound->SetCooperativeLevel( theWindowHandle, DSSCL_NORMAL );
	if ( CheckForError( "[ DSound.Init() ] - SetCoooperativeLevel() Failed!" ) )
		return false;

	hr = lpDirectSound->GetCaps( &Caps );
	if ( CheckForError( "[ DSound.Init() ] - GetCaps() Failed!" ) )
		return false;

	MaxBuffers = 128;
	NumBuffers = 0;

	SoundNames = new string [ MaxBuffers ];

	lpSoundBuffer = new LPDIRECTSOUNDBUFFER [ MaxBuffers ];
	for ( int index = 0; index < MaxBuffers; index++ ) lpSoundBuffer[ index ] = NULL;

	MaxDupBuffers = 32;
	NumDupBuffers = 0;

	lpDupSoundBuffer = new LPDIRECTSOUNDBUFFER [ MaxDupBuffers ];
	for ( index = 0; index < MaxDupBuffers; index++ ) lpDupSoundBuffer[ index ] = NULL;

    // Set up DSBUFFERDESC structure
    DSBUFFERDESC Desc;
    ZeroMemory( &Desc, sizeof( Desc ) );
    Desc.dwSize = sizeof( Desc ); 
    Desc.dwFlags = DSBCAPS_PRIMARYBUFFER; 
    Desc.dwBufferBytes = 0; 
    Desc.lpwfxFormat = NULL; // Must be NULL for primary buffers. 
 
    // Create the primary buffer. 
    hr = lpDirectSound->CreateSoundBuffer( &Desc, &lpPrimaryBuffer, NULL ); 
	if ( CheckForError( "[ DSound.Init() ] - CreateSoundBuffer() Failed!" ) ) return false;

	// Play the primary buffer
	hr = lpPrimaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
	if ( CheckForError( "[ DSound.Init() ] - Play() Failed!" ) ) return false;
	
	return true;
}
