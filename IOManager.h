#pragma once

#include <fstream>
#include "Number.h"

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

	int GetSolutionCount() const { return solutionCount; }
	void SetSolutionCount(int i) { solutionCount = i; }

	std::ifstream inFile;
	std::ofstream outFile;
	std::string fileData;

protected:
	void OpenFile(const std::string fileName, FileType type);
	void PrintFileError(const std::string fileName);
	void SplitData(std::vector<int> & v, const std::string & fileData);
	void CopyData(const std::string & fileData);
	void ClearDataString();

	int solutionCount;
};

