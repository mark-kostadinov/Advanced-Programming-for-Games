#include "IOManager.h"

IOManager::IOManager()
{
}

IOManager::~IOManager()
{
}

/// TODO: Add exception handling for the file I/O
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
			fileData += line + "\n";
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
		int rootInt = root.GetIntegerFromDigits(root.GetDigits());
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

void IOManager::ParseKeyFile(const std::string keyFileName, int & solutionCount, std::vector<Number> & roots, std::vector<Number> & uHashes,
	std::vector<Number> & lHashes, std::vector<Number> & pHashes)
{
	ReadFromFile(keyFileName);

	std::list<std::string> lines;
	SplitData(lines, fileData);

	// Get just the first line which contains the number of solutions in the file
	std::string solutionCountString = lines.front();
	std::vector<int> digitsVector = StringToIntegerVector(solutionCountString);
	solutionCount = Number::GetIntegerFromDigits(digitsVector);
	digitsVector.clear();
	lines.pop_front();

	// Then continue with the rest of the file
	Number num;
	int counter = 0;

	for (std::list<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		digitsVector = StringToIntegerVector(*it);
		num.SetDigits(digitsVector);

		switch (counter)
		{
		case 0:
			roots.push_back(num);
			counter++;
			break;
		case 1:
			uHashes.push_back(num);
			counter++;
			break;
		case 2:
			lHashes.push_back(num);
			counter++;
			break;
		case 3:
			pHashes.push_back(num);
			counter = 0;
			break;
		default:
			PrintToConsole("\nSomething went wrong while trying to switch the counter.", 1);
			break;
		}
		digitsVector.clear();
	}
	ClearDataString();
}

void IOManager::GenerateMultiSafeFileHashes(int & solutionCount, std::vector<Number>& roots, std::vector<Number>& uHashes, std::vector<Number>& lHashes, std::vector<Number>& pHashes, std::vector<Number> & cns, std::vector<Number> & lns, std::vector<Number> & hns)
{
	for (int i = 0; i < solutionCount; i++)
	{
		MultiLockSafe* safe = new MultiLockSafe(numberOfLocksPerSafe, roots.at(i), uHashes.at(i), lHashes.at(i), pHashes.at(i));

		for (int j = 0; j < numberOfLocksPerSafe; j++)
		{
			cns.push_back(safe->GetCombinationLocksVector().at(j)->GetCN());
			lns.push_back(safe->GetCombinationLocksVector().at(j)->GetLN());
			hns.push_back(safe->GetCombinationLocksVector().at(j)->GetHN());
		}
		delete safe;
	}
}

/// TODO: Use the generated multi-safe file to identify if each entry is or is not a valid output given the requirements, which are:
/// => CN must not have repeating digits => NOT VALID solution output label in the multi-safe file
/// => Ensure the sum of the digits on the combination lock to the left is less than the sum of the digits of the combination lock to the right
/// => Ensure that the sum of numbers across all locks is even
std::vector<std::string> IOManager::CheckMultiSafeFileValidity(int & solutionCount, std::vector<Number>& cns, std::vector<Number>& lns, std::vector<Number>& hns)
{
	std::vector<std::string> validityList;
	std::string temp;

	for (std::vector<Number>::iterator it = cns.begin(); it != cns.end(); it++)
	{
		if ((*it).GetDigits().at(0) == (*it).GetDigits().at(1) || (*it).GetDigits().at(0) == (*it).GetDigits().at(2) ||
			(*it).GetDigits().at(0) == (*it).GetDigits().at(3) || (*it).GetDigits().at(1) == (*it).GetDigits().at(2) ||
			(*it).GetDigits().at(1) == (*it).GetDigits().at(3) || (*it).GetDigits().at(2) == (*it).GetDigits().at(3))
			temp = "NOT VALID\n";
		else
			temp = "VALID\n";

		validityList.push_back(temp);
	}

	return validityList;
}

void IOManager::WriteMultiSafeFile(const std::string multiSafeFileName, int & solutionCount, std::vector<Number> & roots, std::vector<Number> & uHashes, std::vector<Number> & lHashes, std::vector<Number> & pHashes)
{
	std::vector<Number> cns;
	std::vector<Number> lns;
	std::vector<Number> hns;

	GenerateMultiSafeFileHashes(solutionCount, roots, uHashes, lHashes, pHashes, cns, lns, hns);
	std::vector<std::string> validityList = CheckMultiSafeFileValidity(solutionCount, cns, lns, hns);

	fileData += "NS " + std::to_string(solutionCount) + "\n";

	for (int i = 0; i < solutionCount; i++)
	{
		fileData += validityList.at(i);

		for (int j = 0; j < numberOfLocksPerSafe; j++)
		{
			fileData += "CN" + std::to_string(j) + " " + std::to_string(cns.at(i).GetIntegerFromDigits(cns.at(i).GetDigits())) + ",";
			fileData += "LN" + std::to_string(j) + " " + std::to_string(lns.at(i).GetIntegerFromDigits(lns.at(i).GetDigits())) + ",";
			fileData += "HN" + std::to_string(j) + " " + std::to_string(hns.at(i).GetIntegerFromDigits(hns.at(i).GetDigits())) + ",\n";
		}
	}
	WriteToFile(multiSafeFileName);
	ClearDataString();
}

void IOManager::GenerateMultiSafeFile(const std::string keyFileName, const std::string multiSafeFileName)
{
	int solutionCount;
	std::vector<Number> roots;
	std::vector<Number> uHashes;
	std::vector<Number> lHashes;
	std::vector<Number> pHashes;

	ParseKeyFile(keyFileName, solutionCount, roots, uHashes, lHashes, pHashes);
	WriteMultiSafeFile(multiSafeFileName, solutionCount, roots, uHashes, lHashes, pHashes);
}

std::vector<int> IOManager::StringToIntegerVector(std::string string)
{
	const char* cstr = string.c_str();
	std::vector<int> digitsVector;
	std::stringstream ss;
	int digit;

	for (int i = 0; i < (int)string.size(); i++)
	{
		if (isdigit(cstr[i]))
		{
			if (cstr[i] == '0')
				digitsVector.push_back(0);
			else
			{
				/// TODO: Fix this
				ss << cstr[i];
				ss >> digit;
				//digit = atoi(&cstr[i]);

				if (cstr[i - 1] == '-')
					digitsVector.push_back(-digit);
				else
					digitsVector.push_back(digit);
			}
		}
	}
	return digitsVector;
}

void IOManager::ClearDataString()
{
	fileData = "";
}

void IOManager::SplitData(std::list<std::string>& line, const std::string & fileData)
{
	std::string delimiter = "\n";
	std::string token;

	int start = 0;
	int end = fileData.find(delimiter);
	while (end != std::string::npos)
	{
		token = fileData.substr(start, end - start);
		line.push_back(token);

		start = end + delimiter.length();
		end = fileData.find(delimiter, start);
	}
}
