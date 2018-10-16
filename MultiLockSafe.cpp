#include "MultiLockSafe.h"

MultiLockSafe::MultiLockSafe(int numLocks)
{
	for (int i = 0; i < numLocks - 1; i++)
	{
		Lock* newLock = new Lock();
		combinationLocksVector.push_back(newLock);
	}
}

MultiLockSafe::~MultiLockSafe()
{
}
