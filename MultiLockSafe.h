#pragma once

#include "Lock.h"

class MultiLockSafe
{
public:
	MultiLockSafe();
	MultiLockSafe(int numLocks);
	MultiLockSafe(int numLocks, Number& root, Number& uHash, Number& pHash, Number& lHash);
	~MultiLockSafe();

	void LockTheSafe();

	std::vector<Lock*> GetCombinationLocksVector() const { return combinationLocksVector; }

protected:
	std::vector<Lock*> combinationLocksVector;
};

