// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616

#include "Timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&start);
}

Timer::~Timer()
{
}

float Timer::GetTimeMS()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&time);
	return (float)((time.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

float Timer::GetTimeSec()
{
	return (float)(GetTimeMS() / 1000.0);
}

void Timer::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&start);
}