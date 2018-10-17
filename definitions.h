#pragma once

#include <iostream>
#include <random>

constexpr int numberOfDigitsPerLock = 4;

// Random-generation functions
/// TODO: Add negative digits
int GenerateRandomDigit();

/// TODO: Add negative numbers
int GenerateRandomFourDigitNumber();
std::vector<int> GenerateRandomFourDigits();

const std::vector<int> cnHash = GenerateRandomFourDigits();
const std::vector<int> lnHash = GenerateRandomFourDigits();
const std::vector<int> hnHash = GenerateRandomFourDigits();

// Printing out function
template <typename T>
inline void PrintToConsole(T element, int numNewLines = NULL)
{
	std::string newLines;
	for (int i = 0; i < numNewLines; i++)
		newLines += "\n";
	std::cout << element << newLines;
}
