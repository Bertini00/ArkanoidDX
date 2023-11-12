#include "Timer.h"

Timer::Timer() {
	lastTime = std::chrono::steady_clock::now();
	dt = std::chrono::duration<float>::zero();
}

void Timer::tick() {
	std::chrono::time_point<std::chrono::steady_clock> currTime = std::chrono::steady_clock::now();
	dt = currTime - lastTime;
	if (dt.count() < 0.0083f)
	{
		dt = std::chrono::duration<float>::zero();
	}
	else
	{
		lastTime = currTime;
	}
}

float Timer::getDeltaTime() 
{ 
	return dt.count(); 
}