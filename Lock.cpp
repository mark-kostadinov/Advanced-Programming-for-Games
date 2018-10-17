#include "Lock.h"

Lock::Lock(Lock* leftLock)
{
	if (leftLock != NULL)
	{
		SetLeftLock(leftLock);
		InitializeLock(leftLock);
	}
	else
		InitializeLock();

	SetIsLocked(false);
}

Lock::~Lock()
{
}

// Each digit of the combination lock may be turned in sequence one movement at a time
void Lock::TurnDigit(int & digit, int times, bool isDigitPositive)
{
	// A positive number will turn a dial downwards and a negative number will turn a dial upwards
	if (!isDigitPositive)
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

/// TODO: the hash function inputs should be the same for all multi-lock safes but the number for the root should be different each time
// Different root for each multi-lock safe or different root for each lock? I'm guessing the first one?
void Lock::InitializeLock(Lock* leftLock)
{
	if (leftLock != NULL)
	{
		root = leftLock->GetRoot(); // Same root for all locks in the multi-lock safe
		UnlockHash(GetFourDigitsFromNumber(leftLock->GetHN()));
		LockHash();
		PassHash();
	}
	else
	{
		GenerateRoot();
		UnlockHash();
		LockHash();
		PassHash();
	}
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

std::vector<int> Lock::GetFourDigitsFromNumber(const int number)
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

void Lock::GenerateRoot()
{
	root = GenerateRandomFourDigitNumber();
}

void Lock::Hash(const std::vector<int> hashDigits, const int* origin, int* derivative)
{
	bool isHashDigitPositive;
	std::vector<int> originDigits = GetFourDigitsFromNumber(*origin);

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (!originDigits.empty() && !hashDigits.empty())
		{
			if (hashDigits.at(i) > 0)
				isHashDigitPositive = true;
			else
				isHashDigitPositive = false;

			TurnDigit(originDigits.at(i), hashDigits.at(i), isHashDigitPositive);
		}
	}
	*derivative = GetNumberFromFourDigits(originDigits);
}

//void Lock::Hash(const int hash, const int* origin, int* derivative)
//{
//	std::vector<int> hashDigits = GetFourDigitsFromNumber(hash);
//	std::vector<int> originDigits = GetFourDigitsFromNumber(*origin);
//
//	for (int i = 0; i < numberOfDigitsPerLock; i++)
//	{
//		if (!originDigits.empty() && !hashDigits.empty())
//		{
//			TurnDigit(originDigits.at(i), hashDigits.at(i), true);
//		}
//	}
//	*derivative = GetNumberFromFourDigits(originDigits);
//}

void Lock::UnlockHash()
{
	Hash(cnHash, &root, &cn);
}

void Lock::UnlockHash(const std::vector<int> hashDigits)
{
	Hash(hashDigits, &root, &cn);
}

void Lock::LockHash()
{
	Hash(lnHash, &cn, &ln);
}

void Lock::PassHash()
{
	Hash(hnHash, &ln, &hn);
}
