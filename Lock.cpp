#include "Lock.h"

Lock::Lock()
{
	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		int randomNumber = GenerateRandomDigit();
		lockDigitsVector.push_back(randomNumber);
	}
	GenerateHashes();

	LockTheLock(true);

	SetIsLocked(false);
}

Lock::~Lock()
{
	//for (std::vector<int*>::iterator it = lockDigitsVector.begin(); it != lockDigitsVector.end(); it++)
	//{
	//	delete (*it);
	//	(*it) = NULL;
	//}
	//lockDigitsVector.clear();
}

int Lock::GenerateRandomDigit() const
{
	return rand() % 10;
}

/// TODO: Find a better solution?
int Lock::GenerateRandomFourDigitNumber() const
{
	int r = rand() % 9999 + 1000;
	while (r > 9999 || r < 1000)
	{
		r = rand() % 9999 + 1000;
	}
	return r;
}

// Each digit of the combination lock may be turned in sequence one movement at a time
void Lock::TurnDigit(int & digit, int times, bool rotateUp)
{
	/// TODO: A positive number will turn a dial downwards and a negative number will turn a dial upwards
	if (!rotateUp)
	{
		for (times; times > 0; times--)
		{
			if (digit == 9)
				digit = 0;
			else
				digit++;
		}
	}
	else
	{
		for (times; times > 0; times--)
		{
			if (digit == 0)
				digit = 9;
			else
				digit--;
		}
	}
}

/// TODO
void Lock::LockTheLock(bool isFirstLock)
{
	if (isFirstLock)
	{
		GenerateRoot();
		UnlockHash();
		LockHash();
		PassHash();
	}
	//else

}

/// TODO: When a button on a combination lock is pressed the lock will either open or remain closed (depending on the combination entered)
void Lock::PressButton()
{
}

int Lock::GetNumberFromFourDigits(std::vector<int> digitsVector)
{
	int multiplier = 1000;
	int currentNumber = 0;

	for (std::vector<int>::iterator it = digitsVector.begin(); it != digitsVector.end(); it++)
	{
		currentNumber += (*it) * multiplier;
		multiplier /= 10;
	}

	return currentNumber;
}

std::vector<int> Lock::GetFourDigitsFromNumber(int number)
{
	std::vector<int> digitsVector;
	std::string temp = std::to_string(number);
	const char *cstr = temp.c_str();

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		int digit;
		std::stringstream ss;

		if (!isdigit(cstr[i]))
		{
			PrintToConsole("Error when trying to convert four digits to a whole number. An input to the digits vector is not a digit.", 1);
		}
		ss << cstr[i];
		ss >> digit;

		digitsVector.push_back(digit);
	}
	return digitsVector;
}

void Lock::GenerateHashes()
{
	cnHash = GenerateRandomFourDigitNumber();
	lnHash = GenerateRandomFourDigitNumber();
	hnHash = GenerateRandomFourDigitNumber();
}

void Lock::GenerateRoot()
{
	root = GenerateRandomFourDigitNumber();
}

void Lock::UnlockHash()
{
	std::vector<int> cnHashDigits = GetFourDigitsFromNumber(cnHash);
	std::vector<int> rootDigits = GetFourDigitsFromNumber(root);

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (!rootDigits.empty() && !cnHashDigits.empty())
		{
			TurnDigit(rootDigits.at(i), cnHashDigits.at(i), true);
		}
	}
	cn = GetNumberFromFourDigits(rootDigits);
}

void Lock::LockHash()
{
	
}

void Lock::PassHash()
{
	
}
