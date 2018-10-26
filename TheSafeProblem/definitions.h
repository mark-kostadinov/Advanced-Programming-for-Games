// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <random>

#define DEBUG
#define FILE_IO_DIRECTORY "../FileIO/"
#define NOT_VALID "NOT VALID\n"
#define VALID "VALID\n"
#define FILE_FORMAT "txt"
#define DOT_FILE_FORMAT ".txt"

constexpr int numberOfDigitsPerLock = 4;
extern int numberOfLocksPerSafe;
extern int lockedLNcounter;

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
inline std::string ToString(const T& value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

template <typename T>
inline void ClearSTLVector(std::vector<T> vector)
{
	vector.clear();
	std::vector<T> temp;
	vector.swap(temp);
}
