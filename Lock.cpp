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

Lock::Lock(Number& root, Number& uHash, Number& pHash, Number& lHash, Lock * leftLock)
{
	if (leftLock != NULL)
	{
		SetLeftLock(leftLock);
		InitializeLock(root, uHash, pHash, lHash, leftLock);
	}
	else
		InitializeLock(root, uHash, pHash, lHash);

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

// The hash function inputs should be the same for all multi-lock safes but the root should be different for each multi-lock safe
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

void Lock::InitializeLock(Number& root, Number& uHash, Number& pHash, Number& lHash, Lock * leftLock)
{
	if (leftLock != NULL)
	{
		this->root = leftLock->root; // Same root for all locks in the multi-lock safe
		UnlockHash(leftLock->GetHN());
		LockHash(lHash);
		PassHash(pHash);
	}
	else
	{
		GenerateRoot(root);
		UnlockHash(uHash);
		LockHash(lHash);
		PassHash(pHash);
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

void Lock::GenerateRoot(const Number & root)
{
	this->root.SetDigits(root.GetDigits());
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
	Hash(cnHash, &root, &cn);
}

void Lock::UnlockHash(const Number & uHash)
{
	Hash(uHash, &root, &cn);
}

void Lock::LockHash()
{
	Hash(lnHash, &cn, &ln);
}

void Lock::LockHash(const Number & lHash)
{
	Hash(lHash, &cn, &ln);
}

void Lock::PassHash()
{
	Hash(hnHash, &ln, &hn);
}

void Lock::PassHash(const Number & pHash)
{
	Hash(pHash, &ln, &hn);
}
