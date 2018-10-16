#include "Lock.h"

Lock::Lock()
{
	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		int* randomNumber = new int(GenerateRandomDigit());
		lockDigitsVector.push_back(randomNumber);
	}
	GenerateHashes();

	LockTheLock(true);

	SetIsLocked(false);
}

Lock::~Lock()
{
	for (std::vector<int*>::iterator it = lockDigitsVector.begin(); it != lockDigitsVector.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}
	lockDigitsVector.clear();

	if (root != NULL)
		delete root;
	root = NULL;
	if (cn != NULL)
		delete cn;
	cn = NULL;
	if (ln != NULL)
		delete ln;
	ln = NULL;
	if (hn != NULL)
		delete hn;
	hn = NULL;
}

int Lock::GenerateRandomDigit() const
{
	return rand() % 10;
}

int Lock::GenerateRandomFourDigitNumber() const
{
	return rand() % 10000 + 1000;
}

// Each digit of the combination lock may be turned in sequence one movement at a time
void Lock::TurnDigit(int * digit, int times, bool rotateUp)
{
	if (!rotateUp)
	{
		for (times; times > 0; times--)
		{
			if (*digit == 9)
				*digit = 0;
			else
				*digit++;
		}
	}
	else
	{
		for (times; times > 0; times--)
		{
			if (*digit == 0)
				*digit = 9;
			else
				*digit--;
		}
	}
}

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

/// When a button on a combination lock is pressed the lock will either open or remain closed (depending on the combination entered)
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

/// TODO: Verify this
std::vector<int> Lock::GetFourDigitsFromNumber(int number)
{
	std::vector<int> digitsVector;
	std::string temp = std::to_string(number);
	const char *cstr = temp.c_str();

	for (int i = numberOfDigitsPerLock; i > 0; i--)
	{
		int digit = (int)cstr[i - 1]; /// TODO: Fix this
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
	root = new int(GenerateRandomFourDigitNumber());
}

void Lock::UnlockHash()
{
	//cn = cnHash on top of the root
	std::vector<int> cnHashDigits = GetFourDigitsFromNumber(cnHash);
	std::vector<int> rootDigits = GetFourDigitsFromNumber(*root);

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (!rootDigits.empty() && !cnHashDigits.empty())
		{
			TurnDigit(&rootDigits.at(i), cnHashDigits.at(i), true);
		}
	}
}

void Lock::LockHash()
{
	
}

void Lock::PassHash()
{
	
}
