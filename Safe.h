#pragma once

#include "Lock.h"

class Safe
{
public:
	Safe();
	virtual ~Safe();

	void LockTheSafe();

	std::vector<Lock*> GetCombinationLocksVector() const { return combinationLocksVector; }

protected:
	std::vector<Lock*> combinationLocksVector;
};

