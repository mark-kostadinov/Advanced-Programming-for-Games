#pragma once

#include "definitions.h"

class Number
{
public:
	Number();
	~Number();

	friend std::ostream & operator << (std::ostream &out, const Number &number);

	// Random-generation functions
	int GenerateRandomPositiveDigit();
	int GenerateRandomDigit();
	int GenerateRandomFourDigitNumber();
	std::vector<int> GenerateRandomFourDigits(bool onlyPositiveDigits = NULL, bool allowZeroFirstDigit = NULL);

	int GetIntegerFromFourDigits(std::vector<int> digitsVector);
	std::vector<int> GetFourDigitsFromInteger(const int number);

	std::vector<int> GetDigits() const { return digits; }
	void SetDigits(std::vector<int> d) { digits = d; }

protected:
	std::vector<int> digits;
};

