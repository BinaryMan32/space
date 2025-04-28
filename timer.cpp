#include "timer.h"

#include <windows.h>

float FrameTime = 0.0f;

void CalcFrameTime()
{
	static timer FrameTimer;
	
	FrameTimer.Stop();
	FrameTime = float( FrameTimer.GetSeconds() );
	FrameTimer.Start();
}

// macro for ReaD TimeStamp Counter
#define RDTSC( var ) \
	_asm _emit 0x0f \
	_asm _emit 0x31 \
	_asm mov DWORD PTR [ var ],     eax \
	_asm mov DWORD PTR [ var + 4 ], edx

// timer class ////////////////////////////////////////////////////////////////

bool   timer::Initialized = false;
double timer::CyclesPerSecond = 0;

void timer::Initialize()
{
	__int64	PerformanceStart, PerformanceFrequency, PerformanceEnd;
	__int64	ClockStart, ClockEnd;

	PerformanceEnd = 0;
	QueryPerformanceFrequency( (LARGE_INTEGER*) &PerformanceFrequency );

	QueryPerformanceCounter( (LARGE_INTEGER*) &PerformanceStart );

	RDTSC( ClockStart );

	while( PerformanceEnd < PerformanceStart + 250000 )
		QueryPerformanceCounter( (LARGE_INTEGER*) &PerformanceEnd );
	
	RDTSC( ClockEnd );

	ClockEnd -= ClockStart;
	CyclesPerSecond = double( ClockEnd ) * double( PerformanceFrequency ) / 250000.0;
}

timer::timer()
{
	if ( !Initialized )
	{
		Initialize();
		Initialized = true;
	}

	StartCycle = 0;
	StopCycle = 0;
	NumCycles = 0;
	NumSeconds = 0;
}

void timer::Start()
{
	_asm
	{
		mov esi, DWORD PTR [this.StartCycle]
		_emit 0x0f
		_emit 0x31
		mov [esi], eax
		mov [esi+4], edx
	}
}

void timer::Stop()
{
	__int64 time;
	
	RDTSC(time)

	StopCycle = time;
	
	if ( StartCycle == 0 )
	{
		NumCycles = 0;
		NumSeconds = 0;
	}
	else
	{
		NumCycles = StopCycle - StartCycle;
		NumSeconds = double( NumCycles ) / CyclesPerSecond;
	}
}

double timer::GetFrequency()
{
	return CyclesPerSecond;	
}

__int64 timer::GetCycles()
{
	return NumCycles;
}

double timer::GetSeconds()
{
	return NumSeconds;
}

