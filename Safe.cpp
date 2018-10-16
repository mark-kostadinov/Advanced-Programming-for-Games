#include "Safe.h"

Safe::Safe()
{
	Lock* defaultLock = new Lock();
	combinationLocksVector.push_back(defaultLock);
}

Safe::~Safe()
{
	for (std::vector<Lock*>::iterator it = combinationLocksVector.begin(); it != combinationLocksVector.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}
	combinationLocksVector.clear();
}

/// TODO: A multi-lock safe consists of a series of 5 combination locks and to lock a safe each combination lock must be locked in turn (lock 0 first through to lock 4)
void Safe::LockTheSafe()
{
	// Reverse iteration required due to the leftmost Lock being at the end of the vector
	for (std::vector<Lock*>::reverse_iterator it = combinationLocksVector.rbegin(); it != combinationLocksVector.rend(); it++)
	{
		//(*it)->LockTheLock();
		
		//if (!(*it)->IsLocked())
			// Repeat
	}
}
