#include "Lock.h"

Lock::Lock(Lock* leftLock)
{
	if (leftLock != NULL)
	{
		SetLeftLock(leftLock);
		root = leftLock->GetRoot(); // Same root for all locks in a multi-lock safe
	}
	else
		GenerateRoot();

	SetIsLocked(false);
}

Lock::~Lock()
{
}

// Each digit of the combination lock may be turned in sequence one movement at a time
void Lock::TurnDigit(int & digit, int times)
{
	int absTimes = abs(times);

	// A positive number will turn a dial downwards and a negative number will turn a dial upwards
	if (times < 0)
	{
		for (absTimes; absTimes > 0; absTimes--)
		{
			if (digit == 9)
				digit = 0;
			else
				digit++;
		}
	}
	else
	{
		for (absTimes; absTimes > 0; absTimes--)
		{
			if (digit == 0)
				digit = 9;
			else
				digit--;
		}
	}
}

/// TODO: The hash function inputs should be the same for all multi-lock safes but the root should be different for each multi-lock safe
void Lock::LockTheLock()
{
	if (GetLeftLock() != NULL)
		GenerateUnlockHash(GetLeftLock()->GetHN());
	else
		GenerateUnlockHash();
	GenerateLockHash();
	GeneratePassHash();

	SetIsLocked(true);
}

// If the default root and hasher values have to be overridden
void Lock::LockTheLock(Number & root, Number & uHash, Number & lHash, Number & pHash)
{
	if (GetLeftLock() != NULL)
	{
		this->root = GetLeftLock()->root; // Same root for all locks in a multi-lock safe
		GenerateUnlockHash(GetLeftLock()->GetHN());
	}
	else
	{
		SetRoot(root);
		GenerateUnlockHash(uHash);
	}
	GenerateLockHash(lHash);
	GeneratePassHash(pHash);

	SetIsLocked(true);
}

/// TODO: When a button on a combination lock is pressed the lock will either open or remain closed (depending on the combination entered)
void Lock::PressButton(const Number guess)
{
}

void Lock::GenerateRoot()
{
	// The root must be a positive number
	root.SetDigits(Number::GenerateRandomFourDigits(true));
}

void Lock::GenerateHash(const Number hash, const Number* origin, Number* derivative)
{
	/// TODO: try catch origin != NULL
	std::vector<int> originDigits = origin->GetDigits();

	for (int i = 0; i < numberOfDigitsPerLock; i++)
	{
		if (!originDigits.empty() && !hash.GetDigits().empty())
			TurnDigit(originDigits.at(i), hash.GetDigits().at(i));
	}
	derivative->SetDigits(originDigits);
}

void Lock::GenerateUnlockHash()
{
	GenerateHash(cnHash, &root, &cn);
}

void Lock::GenerateUnlockHash(const Number & uHash)
{
	GenerateHash(uHash, &root, &cn);
}

void Lock::GenerateLockHash()
{
	GenerateHash(lnHash, &cn, &ln);
}

void Lock::GenerateLockHash(const Number & lHash)
{
	GenerateHash(lHash, &cn, &ln);
}

void Lock::GeneratePassHash()
{
	GenerateHash(hnHash, &ln, &hn);
}

void Lock::GeneratePassHash(const Number & pHash)
{
	GenerateHash(pHash, &ln, &hn);
}

void Lock::SetRoot(const Number & root)
{
	this->root.SetDigits(root.GetDigits());
}
