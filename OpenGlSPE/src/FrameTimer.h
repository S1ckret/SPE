#pragma once

#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	float Mark();
	float GetElapsedTime();
private:
	std::chrono::high_resolution_clock::time_point last;
};