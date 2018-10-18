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
		UnlockHash(leftLock->GetHN());
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

void Lock::GenerateRoot()
{
	// The root must be a positive number
	root.SetDigits(root.GenerateRandomFourDigits(true));
}

void Lock::Hash(const Number hash, const Number* origin, Number* derivative)
{
	bool isHashDigitPositive;
	std::vector<int> originDigits = origin->GetDigits();

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (!originDigits.empty() && !hash.GetDigits().empty())
		{
			if (hash.GetDigits().at(i) > 0)
				isHashDigitPositive = true;
			else
				isHashDigitPositive = false;

			TurnDigit(originDigits.at(i), hash.GetDigits().at(i), isHashDigitPositive);
		}
	}
	derivative->SetDigits(originDigits);
}

void Lock::UnlockHash()
{
	/// TODO: CN must not have repeating digits => Change hash if that occurs and redo
	Hash(cnHash, &root, &cn);
}

void Lock::UnlockHash(const Number hash)
{
	Hash(hash, &root, &cn);
}

void Lock::LockHash()
{
	Hash(lnHash, &cn, &ln);
}

void Lock::PassHash()
{
	Hash(hnHash, &ln, &hn);
}
