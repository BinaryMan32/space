#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

// framerate counter

extern float FrameTime;
void CalcFrameTime();

// timer class

class timer
{
	private:
	
	static bool Initialized;
	static double CyclesPerSecond;

	int64_t StartCycle;
	int64_t StopCycle;
	int64_t NumCycles;
	double  NumSeconds;

	void Initialize();
	
	public:

	// constructor
	timer();

	// Records the Starting time
	void Start();
	
	// Records the Stopping time
	// - can be called multiple times without another StartTime()
	void Stop();

	// Returns the clock frequency
	double GetFrequency();
	
	// Returns the time elapsed between StartTimer() and StopTimer() in clock ticks
	int64_t GetCycles();
	
	// Returns the time elapsed between StartTimer() and StopTimer() in seconds
	double GetSeconds();
};

#endif
