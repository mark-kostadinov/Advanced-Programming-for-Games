#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "definitions.h"

class Lock
{
public:
	Lock(Lock* leftLock = NULL);
	virtual ~Lock();

	void TurnDigit(int& digit, int times, bool isDigitPositive);
	void InitializeLock(Lock* leftLock = NULL);
	void PressButton();
	int GetNumberFromFourDigits(std::vector<int> digitsVector);
	std::vector<int> GetFourDigitsFromNumber(const int number);

	void GenerateRoot();
	void Hash(const std::vector<int> hashDigits, const int* origin, int* derivative);
	void UnlockHash();
	void UnlockHash(const std::vector<int> hashDigits);
	void LockHash();
	void PassHash();

	Lock* GetLeftLock() const { return left; }
	void SetLeftLock(Lock* l) { left = l; }
	int GetRoot() const { return root; }
	int GetCN() const { return cn; }
	int GetLN() const { return ln; }
	int GetHN() const { return hn; }
	bool IsLocked() const { return isLocked; }
	void SetIsLocked(bool b) { isLocked = b; }

protected:
	Lock* left;
	int root;
	int cn;
	int ln;
	int hn;
	bool isLocked;
};
