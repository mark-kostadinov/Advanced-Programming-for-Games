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
			newLock = new Lock();
		else
			newLock = new Lock(combinationLocksVector.at(i - 1));
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

// A multi-lock safe consists of a series of 5 combination locks and to lock a safe each combination lock must be locked in turn (lock 0 first through to lock 4)
void MultiLockSafe::LockTheSafe()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		while (!(*it)->IsLocked())
			(*it)->LockTheLock();
	}
}

void MultiLockSafe::LockTheSafe(Number & root, Number & uHash, Number & lHash, Number & pHash)
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		while (!(*it)->IsLocked())
			(*it)->LockTheLock(root, uHash, lHash, pHash);
	}
}
