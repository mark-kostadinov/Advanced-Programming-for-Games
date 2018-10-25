// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#pragma once

#include "definitions.h"

class Number
{
public:
	Number();
	~Number();

	friend std::ostream & operator << (std::ostream &out, const Number &number);

	// Random-generation functions
	static int GenerateRandomPositiveDigit();
	static int GenerateRandomDigit();
	static int GenerateRandomFourDigitNumber();
	static std::vector<int> GenerateRandomFourDigits(bool onlyPositiveDigits = NULL, bool allowZeroFirstDigit = NULL);
	static std::vector<int> GenerateRandomFourNonDuplicateDigits();
	// Conversion functions
	static std::vector<int> GetFourDigitsFromInteger(const int number);
	static int GetIntegerFromDigits(std::vector<int> digitsVector);
	static std::string GetStringFromDigits(std::vector<int> digitsVector);

	static bool HasDuplicateDigits(const Number &num);
	static bool IsSumOfDigitsBigger(const Number &left, const Number &right);
	static bool IsSumOfDigitsEven(const std::vector<Number> & numVector);

	std::vector<int> GetDigits() const { return digits; }
	void SetDigits(std::vector<int> d) { digits = d; }

protected:
	std::vector<int> digits;
};

