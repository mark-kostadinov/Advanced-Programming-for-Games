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
	combinationLocksVector.clear();
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

void MultiLockSafe::UnlockTheSafe(std::vector<Number> & lockedLNs, std::vector<Number> & unlockedLNs)
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		try
		{
#ifdef DEBUG
			PrintToConsole("Locked File Lock number: " + Number::GetStringFromDigits(lockedLNs.at(lockedLNcounter).GetDigits()), 1);
#endif // DEBUG
			if (!unlockedLNs.empty() && unlockedLNs.size() != numberOfLocksPerSafe)
			{
				for (auto innerIt = unlockedLNs.begin(); innerIt != unlockedLNs.end(); innerIt++)
				{
					// If a Lock is already unlocked, go to the next one
					(*it)->GetLN().SetDigits((*innerIt).GetDigits());
					it++;
				}
			}
#ifdef DEBUG
			PrintToConsole("Generated Lock number guess: " + Number::GetStringFromDigits((*it)->GetLN().GetDigits()), 1);
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
		if (!(*it)->UnlockTheLock(lockedLNs.at(lockedLNcounter)))
			return;
		if (it != combinationLocksVector.end())
		{
			// Get the deduced lock number and use it next time as an input to the LN.at(i)
			unlockedLNs.push_back((*it)->GetLN());
			lockedLNcounter++;
			// Unlock the safe every *numberOfLocksPerSafe* successful iterations
			if (lockedLNcounter != 0 && (lockedLNcounter % numberOfLocksPerSafe == 0))
				break;
			return;
		}
	}
	SetIsLocked(false);
}

void MultiLockSafe::UnlockAllLocks()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
		(*it)->SetIsLocked(false);
}
