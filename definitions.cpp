#include "definitions.h"

int GenerateRandomDigit()
{
	int r = rand() % 10;
	while (r < 0 || r > 10)
		r = rand() % 10;
	return r;
}

int GenerateRandomFourDigitNumber()
{
	int r = rand() % 9999 + 1000;
	while (r < 1000 || r > 9999)
		r = rand() % 9999 + 1000;
	return r;
}

std::vector<int> GenerateRandomFourDigits()
{
	std::vector<int> r;
	for (int i = 0; i < numberOfDigitsPerLock; i++)
		r.push_back(GenerateRandomDigit());
	return r;
}
