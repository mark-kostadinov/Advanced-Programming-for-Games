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
	void UnlockTheSafe(std::vector<Number> & lockedLNs, std::vector<Number> & unlockedLNs);
	void UnlockAllLocks();

	std::vector<Lock*> GetCombinationLocksVector() const { return combinationLocksVector; }
	bool IsLocked() const { return isLocked; }
	void SetIsLocked(bool b) { isLocked = b; }

protected:
	std::vector<Lock*> combinationLocksVector;
	bool isLocked;
};

