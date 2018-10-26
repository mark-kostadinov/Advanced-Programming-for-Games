// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#include "MultiLockSafe.h"

MultiLockSafe::MultiLockSafe()
{
	Lock* defaultLock = new Lock();
	combinationLocksVector.push_back(defaultLock);
	isLocked = false;
}

MultiLockSafe::MultiLockSafe(int numLocks)
{
	for (int i = 0; i < numLocks; i++)
	{
		Lock* newLock;
		if (i == 0)
			newLock = new Lock();
		else
			newLock = new Lock(combinationLocksVector.at(i - 1));
		combinationLocksVector.push_back(newLock);
	}
	isLocked = false;
}

MultiLockSafe::~MultiLockSafe()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}
	ClearSTLVector(combinationLocksVector);
}

// A multi-lock safe consists of a series of 5 combination locks and to lock a safe each combination lock must be locked in turn (lock 0 first through to lock 4)
void MultiLockSafe::LockTheSafe()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		while (!(*it)->IsLocked())
			(*it)->LockTheLock();
	}
	isLocked = true;
}

void MultiLockSafe::LockTheSafe(Number & root, Number & uHash, Number & lHash, Number & pHash)
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		while (!(*it)->IsLocked())
			(*it)->LockTheLock(root, uHash, lHash, pHash);
	}
	isLocked = true;
}

void MultiLockSafe::UnlockTheSafe(std::vector<Number> & lockedLNs, const bool hasBonusMultiSafe)
{
	try
	{
#ifdef DEBUG
		PrintToConsole("Locked File Lock number: " + Number::GetStringFromDigits(lockedLNs.at(lockedLNcounter).GetDigits()), 1);
		PrintToConsole("Generated Lock number guess: " + Number::GetStringFromDigits(combinationLocksVector.at(0)->GetLN().GetDigits()), 1);
#endif // DEBUG
	}
	catch (const std::out_of_range& oor)
	{
		std::string exceptionString = "Out of range exception caught: ";
		exceptionString += oor.what();
		PrintToConsole(exceptionString, 1);
		exit(1);
	}
	// If the lock is not unlocked go back and try again
	if (!combinationLocksVector.at(0)->UnlockTheLock(lockedLNs.at(lockedLNcounter)))
		return;

	if (IsSafeValid(hasBonusMultiSafe))
	{
		lockedLNcounter += numberOfLocksPerSafe;
		SetIsLocked(false);
	}
}

void MultiLockSafe::UnlockAllLocks()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
		(*it)->SetIsLocked(false);
}

bool MultiLockSafe::IsSafeValid(const bool hasBonusMultiSafe)
{
	int counter = -1;

	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		// CNs must not have repeating digits
		if (Number::HasDuplicateDigits((*it)->GetCN()))
		{
#ifdef DEBUG
			PrintToConsole("Invalid possible CN: " + Number::GetStringFromDigits((*it)->GetCN().GetDigits()), 1);
#endif // DEBUG
			return false;
		}
		// Ensure the sum of the digits on the combination lock to the left is less than the sum of the digits of the combination lock to the right
		if (hasBonusMultiSafe)
		{
			counter++;
			if (it != combinationLocksVector.end() && counter < numberOfLocksPerSafe - 1)
			{
				if (Number::IsSumOfDigitsBigger((*it)->GetCN(), (*(it + 1))->GetCN()))
					return false;
			}
			else
			{
				if (Number::IsSumOfDigitsBigger((*(it - 1))->GetCN(), (*it)->GetCN()))
					return false;
			}
		}
	}
	return true;
}
