// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#include "Number.h"

Number::Number()
{
	digits = GenerateRandomFourDigits();
}

Number::~Number()
{
}

std::ostream & operator<<(std::ostream & out, const Number & number)
{
	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (number.GetDigits().at(i) == 0)
			out << " " << number.GetDigits().at(i) << ",";
		else if (number.GetDigits().at(i) > 0)
			out << "+" << number.GetDigits().at(i) << ",";
		else
			out << number.GetDigits().at(i) << ",";
	}
	return out;
}

int Number::GenerateRandomPositiveDigit()
{
	int r = rand() % 10;
	while (r < 0 || r > 9)
		r = rand() % 10;
	return r;
}

int Number::GenerateRandomDigit()
{
	int r = (rand() % 10) - (rand() % 10);
	while (r < -9 || r > 9)
		r = (rand() % 10) - (rand() % 10);
	return r;
}

int Number::GenerateRandomFourDigitNumber()
{
	int r = rand() % 9999 + 1000;
	while (r < 1000 || r > 9999)
		r = rand() % 9999 + 1000;
	return r;
}

std::vector<int> Number::GenerateRandomFourNonDuplicateDigits()
{
	std::vector<int> r;
	int digit;

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		digit = GenerateRandomDigit();
		for (auto it = r.begin(); it != r.end(); it++)
		{
			while (!(abs(digit) != abs(*it)))
			{
				digit = GenerateRandomDigit();
				it = r.begin();
			}
		}
		r.push_back(digit);
	}
	return r;
}

std::vector<int> Number::GenerateRandomFourDigits(bool onlyPositiveDigits, bool allowZeroFirstDigit)
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

std::vector<int> Number::GetFourDigitsFromInteger(const int number)
{
	std::vector<int> digitsVector;
	int digit;
	std::stringstream ss;
	std::string temp = std::to_string(number);
	const char *cstr = temp.c_str();

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		try
		{
			if (!isdigit(cstr[i]))
			{
				throw ("\nError when trying to convert four digits to a whole number. An input to the digits vector is not a digit.");
			}
		}
		catch (const char* msg)
		{
			std::cerr << msg << std::endl;
		}
		ss << cstr[i];
		ss >> digit;

		digitsVector.push_back(digit);
	}
	return digitsVector;
}

int Number::GetIntegerFromDigits(std::vector<int> digitsVector)
{
	int multiplier = 1;
	for (int i = 1; i < (int)digitsVector.size(); i++)
		multiplier *= 10;
	int currentNumber = 0;

	for (std::vector<int>::iterator it = digitsVector.begin(); it != digitsVector.end(); it++)
	{
		currentNumber += (*it) * multiplier;
		multiplier /= 10;
	}
	return currentNumber;
}

std::string Number::GetStringFromDigits(std::vector<int> digitsVector)
{
	std::string result;

	for (std::vector<int>::iterator it = digitsVector.begin(); it != digitsVector.end(); it++)
	{
		result += ToString(*it);
	}
	return result;
}

bool Number::HasDuplicateDigits(const Number & num)
{
	if (num.GetDigits().at(0) == num.GetDigits().at(1) || num.GetDigits().at(0) == num.GetDigits().at(2) ||
		num.GetDigits().at(0) == num.GetDigits().at(3) || num.GetDigits().at(1) == num.GetDigits().at(2) ||
		num.GetDigits().at(1) == num.GetDigits().at(3) || num.GetDigits().at(2) == num.GetDigits().at(3))
		return true;
	else
		return false;
}

bool Number::IsSumOfDigitsBigger(const Number & left, const Number & right)
{
	int sumLeft = 0, sumRight = 0;

	for (auto it = left.digits.begin(); it != left.digits.end(); it++)
		sumLeft += (*it);
	for (auto it = right.digits.begin(); it != right.digits.end(); it++)
		sumRight += (*it);
	
	if (sumLeft >= sumRight)
		return true;
	else
		return false;
}

bool Number::IsSumOfDigitsEven(const std::vector<Number>& numVector)
{
	int sumDigits = 0;

	for (auto it = numVector.begin(); it != numVector.end(); it++)
	{
		for (int i = 0; i < numberOfDigitsPerLock; i++)
			sumDigits += (*it).GetDigits().at(i);
	}

	if (sumDigits != 0 && (sumDigits % 2 == 0))
		return true;
	else
		return false;
}
