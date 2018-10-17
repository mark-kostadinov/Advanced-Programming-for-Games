#pragma once

#include <iostream>

constexpr int numberOfDigitsPerLock = 4;

// Printing out function
template <typename T>
void PrintToConsole(T element, int numNewLines = NULL)
{
	std::string newLines;
	for (int i = 0; i < numNewLines; i++)
		newLines += "\n";
	std::cout << element << newLines;
}