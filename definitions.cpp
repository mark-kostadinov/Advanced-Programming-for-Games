#include "definitions.h"

int GenerateRandomPositiveDigit()
{
	int r = rand() % 10;
	while (r < 0 || r > 9)
		r = rand() % 10;
	return r;
}

int GenerateRandomDigit()
{
	int r = (rand() % 10) - (rand() % 10);
	while (r < -9 || r > 9)
		r = (rand() % 10) - (rand() % 10);
	return r;
}

int GenerateRandomFourDigitNumber()
{
	int r = rand() % 9999 + 1000;
	while (r < 1000 || r > 9999)
		r = rand() % 9999 + 1000;
	return r;
}

std::vector<int> GenerateRandomFourDigits(bool onlyPositiveDigits, bool allowZeroFirstDigit)
{
	std::vector<int> r;
	int digit;

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (onlyPositiveDigits)
		{
			digit = GenerateRandomPositiveDigit();
			while (!allowZeroFirstDigit && i == 0 && digit == 0)
				digit = GenerateRandomPositiveDigit();
		}
		else
		{
			digit = GenerateRandomDigit();
			while (!allowZeroFirstDigit && i == 0 && digit == 0)
				digit = GenerateRandomDigit();
		}
		r.push_back(digit);
	}
	return r;
}
