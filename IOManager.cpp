#include "IOManager.h"

IOManager::IOManager()
{
}

IOManager::~IOManager()
{
}

void IOManager::OpenFile(const std::string fileName, FileType type)
{
	if (type == READ_IN)
	{
		inFile.open(INPUT_DIRECTORY + fileName);
		if (!inFile)
			PrintFileError(fileName);
	}
	else if (type == WRITE_OUT)
	{
		outFile.open(OUTPUT_DIRECTORY + fileName);
		if (!outFile)
			PrintFileError(fileName);
	}
}

void IOManager::PrintFileError(const std::string fileName)
{
	PrintToConsole("An error was encountered while trying to open ");
	PrintToConsole(fileName, 1);
	exit(-1);
}

void IOManager::WriteToFile(const std::string fileName)
{
	OpenFile(fileName, WRITE_OUT);

	if (outFile.is_open())
	{
		/// TODO: While there are still parameters to output
		outFile << fileData;
	}
	outFile.close();
}

void IOManager::ReadFromFile(const std::string fileName)
{
	std::string line;

	OpenFile(fileName, READ_IN);

	// Copy the data in while the file is open, there is still more content that has not been covered and we have not reached its end
	if (inFile.is_open())
	{
		while (getline(inFile, line) && !inFile.eof())
			fileData += line;
	}
	inFile.close();
}

/// TODO: Make it look better
void IOManager::GenerateKeyFile(const std::string fileName, int solutionCount)
{
	fileData += "NS " + std::to_string(solutionCount) + "\n";

	for (int i = 0; i < solutionCount; i++)
	{
		Number root;
		root.SetDigits(root.GenerateRandomFourDigits(true));
		int rootInt = root.GetIntegerFromFourDigits(root.GetDigits());
		fileData += "ROOT " + ToString(rootInt) + "\n";

		Number uhash;
		fileData += "UHF " + ToString(uhash) + "\n";

		Number lhash;
		fileData += "LHF " + ToString(lhash) + "\n";

		Number phash;
		fileData += "PHF " + ToString(phash) + "\n";
	}
	WriteToFile(fileName);
	ClearDataString();
}

void IOManager::ClearDataString()
{
	fileData = "";
}

/// TODO: Are both these needed?
void IOManager::SplitData(std::vector<int>& v, const std::string & fileData)
{
	std::string delimiter = ",";
	std::string token;
	int value;

	int start = 0;
	int end = fileData.find(delimiter);
	while (end != std::string::npos)
	{
		token = fileData.substr(start, end - start);
		value = atoi(token.c_str());
		v.push_back(value);

		start = end + delimiter.length();
		end = fileData.find(delimiter, start);
	}
}

void IOManager::CopyData(const std::string & fileData)
{
	// Split the text into tokens
	std::vector<int> v;
	SplitData(v, fileData);

	//SetTotalMobs(v.back());
	//v.pop_back();
}

