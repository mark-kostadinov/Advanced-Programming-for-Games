#pragma once

#include "Number.h"

const Number cnHash;
const Number lnHash;
const Number hnHash;

class Lock
{
public:
	Lock(Lock* leftLock = NULL);
	Lock(Number& root, Number& uHash, Number& pHash, Number& lHash, Lock* leftLock = NULL);
	virtual ~Lock();

	void TurnDigit(int& digit, int times, bool isDigitPositive);
	void InitializeLock(Lock* leftLock = NULL);
	void InitializeLock(Number& root, Number& uHash, Number& pHash, Number& lHash, Lock* leftLock = NULL);
	void PressButton();

	void GenerateRoot();
	void GenerateRoot(const Number & root);
	void Hash(const Number hash, const Number* origin, Number* derivative);
	void UnlockHash();
	void UnlockHash(const Number & uHash);
	void LockHash();
	void LockHash(const Number & lHash);
	void PassHash();
	void PassHash(const Number & pHash);

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
