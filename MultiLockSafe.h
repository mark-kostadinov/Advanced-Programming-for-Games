#pragma once

#include "Safe.h"

class MultiLockSafe : public Safe
{
public:
	MultiLockSafe(int numLocks);
	virtual ~MultiLockSafe();
};

