#pragma once

#include "Number.h"

const Number cnHash;
const Number lnHash;
const Number hnHash;

class Lock
{
public:
	Lock(Lock* leftLock = NULL);
	virtual ~Lock();

	void TurnDigit(int& digit, int times);
	void LockTheLock();
	void LockTheLock(Number& root, Number& uHash, Number& lHash, Number& pHash);
	bool UnlockTheLock(const Number & guess);

	void GenerateRoot();
	void GenerateHash(const Number hash, const Number* origin, Number* derivative);
	void GenerateUnlockHash();
	void GenerateUnlockHash(const Number & uHash);
	void GenerateUnlockHash(const Number & uHash, const Number & leftHN);
	void GenerateLockHash();
	void GenerateLockHash(const Number & lHash);
	void GeneratePassHash();
	void GeneratePassHash(const Number & pHash);

	Lock* GetLeftLock() const { return left; }
	void SetLeftLock(Lock* l) { left = l; }
	Number GetRoot() const { return root; }
	void SetRoot(const Number & root);
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
