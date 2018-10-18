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
	void Hash(const std::vector<int> hashDigits, const std::vector<int>* origin, std::vector<int>* derivative);
	void UnlockHash();
	void UnlockHash(const std::vector<int> hashDigits);
	void LockHash();
	void PassHash();

	Lock* GetLeftLock() const { return left; }
	void SetLeftLock(Lock* l) { left = l; }
	std::vector<int> GetRoot() const { return root; }
	std::vector<int> GetCN() const { return cn; }
	std::vector<int> GetLN() const { return ln; }
	std::vector<int> GetHN() const { return hn; }
	bool IsLocked() const { return isLocked; }
	void SetIsLocked(bool b) { isLocked = b; }

protected:
	Lock* left;
	std::vector<int> root;
	std::vector<int> cn;
	std::vector<int> ln;
	std::vector<int> hn;
	bool isLocked;
};
