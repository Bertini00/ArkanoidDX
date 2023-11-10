#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float getDeltaTime();
	void tick();

private:
	std::chrono::time_point<std::chrono::steady_clock> lastTime;
	std::chrono::duration<double> dt;

};

