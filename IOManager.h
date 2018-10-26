// Code written for CSC8501 by Mark Kostadinov, Student Number 150368616
#pragma once

#include <fstream>
#include <stdexcept>
#include "Number.h"
#include "MultiLockSafe.h"
#include "Timer.h"

enum FileType
{
	READ_IN, WRITE_OUT
};

class IOManager
{
public:
	IOManager();
	~IOManager();

	void WriteToFile(const std::string fileName);
	void ReadFromFile(const std::string fileName);
	void GenerateKeyFile(const std::string fileName, int solutionCount, const bool onlyValidSolutions = NULL);
	void GenerateMultiSafeFile(const std::string keyFileName, const std::string multiSafeFileName, const bool hasBonusMultiSafe = NULL);
	std::vector<int> StringToIntegerVector(std::string string);
	void GenerateLockedSafeFile(const std::string lockedSafeFileName);
	void UnlockLockedSafeFile(const std::string lockedSafeFileName, const bool hasBonusMultiSafe = NULL);

	void ChangeNumberOfLocksPerSafe();
	void GenerateKeyFileUI();
	void GenerateMultiSafeFileUI();
	void GenerateLockedSafeFileUI();
	void UnlockSafeFileUI();

	Timer* GetPerformanceTimer() const { return perfTimer; }

	int GetSolutionCount() const { return solutionCount; }
	void SetSolutionCount(int i) { solutionCount = i; }
	int GetLockedSolutionCount() const { return lockedSolutionsCount; }
	void SetLockedSolutionCount(int i) { lockedSolutionsCount = i; }

	std::vector<Number> GetRoots() const { return roots; }
	std::vector<Number> GetUHashes() const { return uHashes; }
	std::vector<Number> GetLockedUHashes() const { return lockedUHashes; }
	std::vector<Number> GetLHashes() const { return lHashes; }
	std::vector<Number> GetLockedLHashes() const { return lockedLHashes; }
	std::vector<Number> GetPHashes() const { return pHashes; }
	std::vector<Number> GetLockedPHashes() const { return lockedPHashes; }
	std::vector<Number> GetCNs() const { return cns; }
	std::vector<Number> GetLNs() const { return lns; }
	std::vector<Number> GetHNs() const { return hns; }
	std::vector<std::string> GetValidityList() const { return validityList; }

protected:
	void OpenFile(const std::string fileName, FileType type);
	void SplitData(std::list<std::string> & line, const std::string & fileData);
	void ParseKeyFile(const std::string keyFileName);
	void ParseLockedSafeFile(const std::string lockedSafeFileName);
	void ParseSolutionCount(std::list<std::string> & lines, std::vector<int> & digitsVector);
	void GenerateMultiSafeFileHashes();
	void CheckMultiSafeFileValidity(std::vector<std::string> & validityList, const bool hasBonusMultiSafe = NULL);
	void WriteMultiSafeFile(const std::string multiSafeFileName, const bool hasBonusMultiSafe = NULL);
	void VerifyFileFormat(std::string & fileName);
	std::string GetLastThreeCharsFromString(const std::string & string);
	void ClearDataString(std::string & dataString);
	void ClearDataStructures();
	void UnlockUsingRNG(const bool hasBonusMultiSafe = NULL);
	void GenerateKeyFileFromLockedFile(const std::string fileName);

	Timer* perfTimer;

	int solutionCount;
	int lockedSolutionsCount;

	std::vector<Number> roots;
	std::vector<Number> uHashes;
	std::vector<Number> lockedUHashes;
	std::vector<Number> lHashes;
	std::vector<Number> lockedLHashes;
	std::vector<Number> pHashes;
	std::vector<Number> lockedPHashes;
	std::vector<Number> cns;
	std::vector<Number> lns;
	std::vector<Number> hns;
	std::vector<std::string> validityList;

	std::ifstream inFile;
	std::ofstream outFile;
	std::string fileData;
	std::string lockedFileData;
};

