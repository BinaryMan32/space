#include "timer.h"

#include <chrono>

float FrameTime = 0.0f;

void CalcFrameTime()
{
	static timer FrameTimer;
	
	FrameTimer.Stop();
	FrameTime = float( FrameTimer.GetSeconds() );
	FrameTimer.Start();
}

// Reads a monotonic clock in nanoseconds
static int64_t ReadClock()
{
	return std::chrono::duration_cast< std::chrono::nanoseconds >(
		std::chrono::steady_clock::now().time_since_epoch() ).count();
}

// timer class ////////////////////////////////////////////////////////////////

bool   timer::Initialized = false;
double timer::CyclesPerSecond = 0;

void timer::Initialize()
{
	CyclesPerSecond = 1.0e9;
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
	StartCycle = ReadClock();
}

void timer::Stop()
{
	StopCycle = ReadClock();
	
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

int64_t timer::GetCycles()
{
	return NumCycles;
}

double timer::GetSeconds()
{
	return NumSeconds;
}
