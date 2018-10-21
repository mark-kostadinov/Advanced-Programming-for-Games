#pragma once

#include "Lock.h"

class MultiLockSafe
{
public:
	MultiLockSafe();
	MultiLockSafe(int numLocks);
	~MultiLockSafe();

	void LockTheSafe();
	void LockTheSafe(Number& root, Number& uHash, Number& lHash, Number& pHash);

	std::vector<Lock*> GetCombinationLocksVector() const { return combinationLocksVector; }

protected:
	std::vector<Lock*> combinationLocksVector;
};

