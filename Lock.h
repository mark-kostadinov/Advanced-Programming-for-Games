#pragma once

#include <random>
#include <vector>
#include <string>
#include <sstream>
#include "definitions.h"

class Lock
{
public:
	Lock();
	virtual ~Lock();

	int GenerateRandomDigit() const;
	int GenerateRandomFourDigitNumber() const;
	void TurnDigit(int& digit, int times, bool rotateUp);
	void LockTheLock(bool isFirstLock);
	void PressButton();
	void GenerateHashes();
	void GenerateRoot();
	void UnlockHash();
	void LockHash();
	void PassHash();

	std::vector<int> GetLockDigitsVector() const { return lockDigitsVector; }

	int GetNumberFromFourDigits(std::vector<int> digitsVector);
	std::vector<int> GetFourDigitsFromNumber(int number);

	int GetCNHash() const { return cnHash; }
	int GetLNHash() const { return lnHash; }
	int GetHNHash() const { return hnHash; }
	int GetRoot() const { return root; }
	int GetCN() const { return cn; }
	int GetLN() const { return ln; }
	int GetHN() const { return hn; }

	bool IsLocked() const { return isLocked; }
	void SetIsLocked(bool b) { isLocked = b; }

protected:
	std::vector<int> lockDigitsVector;

	int cnHash;
	int lnHash;
	int hnHash;
	int root;
	int cn;
	int ln;
	int hn;

	bool isLocked;
};
