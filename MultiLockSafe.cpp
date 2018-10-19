#include "MultiLockSafe.h"

MultiLockSafe::MultiLockSafe()
{
	Lock* defaultLock = new Lock();
	combinationLocksVector.push_back(defaultLock);
}

MultiLockSafe::MultiLockSafe(int numLocks)
{
	for (int i = 0; i < numLocks; i++)
	{
		Lock* newLock;
		if (i == 0)
		{
			newLock = new Lock();
		}
		else
			newLock = new Lock(combinationLocksVector.at(i));
		combinationLocksVector.push_back(newLock);
	}
}

MultiLockSafe::MultiLockSafe(int numLocks, Number& root, Number& uHash, Number& pHash, Number& lHash)
{
	for (int i = 0; i < numLocks; i++)
	{
		Lock* newLock;
		if (i == 0)
		{
			newLock = new Lock(root, uHash, pHash, lHash);
		}
		else
			newLock = new Lock(root, uHash, pHash, lHash, combinationLocksVector.at(i));
		combinationLocksVector.push_back(newLock);
	}
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

/// TODO: A multi-lock safe consists of a series of 5 combination locks and to lock a safe each combination lock must be locked in turn (lock 0 first through to lock 4)
void MultiLockSafe::LockTheSafe()
{
	// Reverse iteration required due to the leftmost Lock being at the end of the vector
	for (std::vector<Lock*>::reverse_iterator it = combinationLocksVector.rbegin(); it != combinationLocksVector.rend(); it++)
	{
		//(*it)->LockTheLock();

		//if (!(*it)->IsLocked())
			// Repeat
	}
}
