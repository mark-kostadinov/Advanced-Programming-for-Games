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

	std::vector<int> GetDigits() const { return digits; }
	void SetDigits(std::vector<int> d) { digits = d; }

protected:
	std::vector<int> digits;
};

