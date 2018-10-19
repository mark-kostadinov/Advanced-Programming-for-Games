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

	int GetSolutionCount() const { return solutionCount; }
	void SetSolutionCount(int i) { solutionCount = i; }

	std::ifstream inFile;
	std::ofstream outFile;
	std::string fileData;

protected:
	void OpenFile(const std::string fileName, FileType type);
	void PrintFileError(const std::string fileName);
	void SplitData(std::list<std::string> & line, const std::string & fileData);
	void ParseKeyFile(const std::string keyFileName, int & solutionCount, std::vector<Number> & roots, std::vector<Number> & uHashes,
		std::vector<Number> & lHashes, std::vector<Number> & pHashes);
	void GenerateMultiSafeFileHashes(int & solutionCount, std::vector<Number>& roots, std::vector<Number>& uHashes, std::vector<Number>& lHashes, std::vector<Number>& pHashes, std::vector<Number> & cns, std::vector<Number> & lns, std::vector<Number> & hns);
	std::vector<std::string> CheckMultiSafeFileValidity(int & solutionCount, std::vector<Number>& cns, std::vector<Number>& lns, std::vector<Number>& hns);
	void WriteMultiSafeFile(const std::string multiSafeFileName, int & solutionCount, std::vector<Number> & roots, std::vector<Number> & uHashes, std::vector<Number> & lHashes, std::vector<Number> & pHashes);
	void ClearDataString();

	int solutionCount;
};

