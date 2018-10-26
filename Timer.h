// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#pragma once

#include <time.h>
#include <Windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	float GetTimeMS();
	void Reset();

protected:
	LARGE_INTEGER start;
	LARGE_INTEGER frequency;
	LARGE_INTEGER time;
};
