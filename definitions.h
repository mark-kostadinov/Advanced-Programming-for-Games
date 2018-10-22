#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <random>

#define INPUT_DIRECTORY "../Input/"
#define OUTPUT_DIRECTORY "../Output/"
#define NOT_VALID "NOT VALID\n"
#define VALID "VALID\n"

constexpr int numberOfDigitsPerLock = 4;
constexpr int numberOfLocksPerSafe = 5;

// Printing out function
template <typename T>
inline void PrintToConsole(T element, int numNewLines = NULL)
{
	std::string newLines;
	for (int i = 0; i < numNewLines; i++)
		newLines += "\n";
	std::cout << element << newLines;
}

// Thing to string function
template <typename T>
std::string ToString(const T& value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}
