#pragma once

#include <fstream>
#include <stdexcept>
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
	void UnlockLockedSafeFile(const std::string lockedSafeFileName);

	void ChangeNumberOfLocksPerSafe();
	void GenerateKeyFileUI();
	void GenerateMultiSafeFileUI();
	void GenerateLockedSafeFileUI();
	void UnlockSafesUI();

	int GetSolutionCount() const { return solutionCount; }
	void SetSolutionCount(int i) { solutionCount = i; }
	int GetLockedSolutionCount() const { return lockedSolutionsCount; }
	void SetLockedSolutionCount(int i) { lockedSolutionsCount = i; }

	/// TODO: Do I really need all of these?
	std::vector<Number> GetRoots() const { return roots; }
	std::vector<Number> GetLockedRoots() const { return locked_roots; }
	std::vector<Number> GetUHashes() const { return uHashes; }
	std::vector<Number> GetLockedUHashes() const { return locked_uHashes; }
	std::vector<Number> GetLHashes() const { return lHashes; }
	std::vector<Number> GetLockedLHashes() const { return locked_lHashes; }
	std::vector<Number> GetPHashes() const { return pHashes; }
	std::vector<Number> GetLockedPHashes() const { return locked_pHashes; }
	std::vector<Number> GetCNs() const { return cns; }
	std::vector<Number> GetLockedCNs() const { return locked_cns; }
	std::vector<Number> GetLNs() const { return lns; }
	std::vector<Number> GetLockedLNs() const { return locked_lns; }
	std::vector<Number> GetHNs() const { return hns; }
	std::vector<Number> GetLockedHNs() const { return locked_hns; }
	std::vector<std::string> GetValidityList() const { return validityList; }

protected:
	void OpenFile(const std::string fileName, FileType type);
	void SplitData(std::list<std::string> & line, const std::string & fileData);
	void ParseKeyFile(const std::string keyFileName);
	void ParseLockedSafeFile(const std::string lockedSafeFileName);
	void ParseSolutionCount(std::list<std::string> & lines, std::vector<int> & digitsVector);
	void GenerateMultiSafeFileHashes();
	void CheckMultiSafeFileValidity(std::vector<std::string> & validityList);
	void WriteMultiSafeFile(const std::string multiSafeFileName);
	void VerifyFileFormat(std::string & fileName);
	std::string GetLastThreeCharsFromString(const std::string & string);
	void ClearDataString(std::string & dataString);

	std::ifstream inFile;
	std::ofstream outFile;
	std::string fileData;
	std::string lockedFileData;

	int solutionCount;
	int lockedSolutionsCount;

	std::vector<Number> roots;
	std::vector<Number> locked_roots;
	std::vector<Number> uHashes;
	std::vector<Number> locked_uHashes;
	std::vector<Number> lHashes;
	std::vector<Number> locked_lHashes;
	std::vector<Number> pHashes;
	std::vector<Number> locked_pHashes;
	std::vector<Number> cns;
	std::vector<Number> locked_cns;
	std::vector<Number> lns;
	std::vector<Number> locked_lns;
	std::vector<Number> hns;
	std::vector<Number> locked_hns;
	std::vector<std::string> validityList;
};

