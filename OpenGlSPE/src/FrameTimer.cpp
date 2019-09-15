#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last = std::chrono::high_resolution_clock::now();
}

float FrameTimer::Mark()
{
	const auto old = last;
	last = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> frameTime = last - old; 
	return frameTime.count();
}

float FrameTimer::GetElapsedTime()
{
	const std::chrono::duration<float> elapsedTime = last.time_since_epoch(); 
	return elapsedTime.count();
}
