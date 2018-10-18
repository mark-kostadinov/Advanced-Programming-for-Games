#pragma once

#include "IOManager.h"

const Number cnHash;
const Number lnHash;
const Number hnHash;

class Lock
{
public:
	Lock(Lock* leftLock = NULL);
	virtual ~Lock();

	void TurnDigit(int& digit, int times, bool isDigitPositive);
	void InitializeLock(Lock* leftLock = NULL);
	void PressButton();

	void GenerateRoot();
	void Hash(const Number hash, const Number* origin, Number* derivative);
	void UnlockHash();
	void UnlockHash(const Number hash);
	void LockHash();
	void PassHash();

	Lock* GetLeftLock() const { return left; }
	void SetLeftLock(Lock* l) { left = l; }
	Number GetRoot() const { return root; }
	Number GetCN() const { return cn; }
	Number GetLN() const { return ln; }
	Number GetHN() const { return hn; }
	bool IsLocked() const { return isLocked; }
	void SetIsLocked(bool b) { isLocked = b; }

protected:
	Lock* left;
	Number root;
	Number cn;
	Number ln;
	Number hn;
	bool isLocked;
};
