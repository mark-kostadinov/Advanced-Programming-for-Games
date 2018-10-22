#pragma once

#include <fstream>
#include "Number.h"
#include "MultiLockSafe.h"

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
	void GenerateKeyFile(const std::string fileName, int solutionCount);
	void GenerateMultiSafeFile(const std::string keyFileName, const std::string multiSafeFileName);
	std::vector<int> StringToIntegerVector(std::string string);
	void GenerateLockedSafeFile(const std::string lockedSafeFileName);

	int GetSolutionCount() const { return solutionCount; }
	void SetSolutionCount(int i) { solutionCount = i; }
	int GetLockedSolutionCount() const { return lockedSolutionsCount; }
	void SetLockedSolutionCount(int i) { lockedSolutionsCount = i; }

	std::vector<Number> GetRoots() const { return roots; }
	std::vector<Number> GetUHashes() const { return uHashes; }
	std::vector<Number> GetLHashes() const { return lHashes; }
	std::vector<Number> GetPHashes() const { return pHashes; }
	std::vector<Number> GetCNs() const { return cns; }
	std::vector<Number> GetLNs() const { return lns; }
	std::vector<Number> GetHNs() const { return hns; }
	std::vector<std::string> GetValidityList() const { return validityList; }

protected:
	void OpenFile(const std::string fileName, FileType type);
	void PrintFileError(const std::string fileName);
	void SplitData(std::list<std::string> & line, const std::string & fileData);
	void ParseKeyFile(const std::string keyFileName);
	void GenerateMultiSafeFileHashes();
	void CheckMultiSafeFileValidity(std::vector<std::string> & validityList);
	void WriteMultiSafeFile(const std::string multiSafeFileName);
	void ClearDataString();

	std::ifstream inFile;
	std::ofstream outFile;
	std::string fileData;
	std::string lockedFileData;

	int solutionCount;
	int lockedSolutionsCount;

	std::vector<Number> roots;
	std::vector<Number> uHashes;
	std::vector<Number> lHashes;
	std::vector<Number> pHashes;
	std::vector<Number> cns;
	std::vector<Number> lns;
	std::vector<Number> hns;
	std::vector<std::string> validityList;
};

