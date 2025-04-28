#ifndef TIMER_H
#define TIMER_H

// framerate counter

extern float FrameTime;
void CalcFrameTime();

// timer class

class timer
{
	private:
	
	static bool Initialized;
	static double CyclesPerSecond;

	__int64 StartCycle;
	__int64 StopCycle;
	__int64 NumCycles;
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

	// Returns the processor frequency
	double GetFrequency();
	
	// Returns the time elapsed between StartTimer() and StopTimer() in cycles
	__int64 GetCycles();
	
	// Returns the time elapsed between StartTimer() and StopTimer() in seconds
	double GetSeconds();
};

#endif
