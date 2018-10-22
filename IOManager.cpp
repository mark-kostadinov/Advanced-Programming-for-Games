#include "IOManager.h"

IOManager::IOManager()
{
	solutionCount = 0;
	lockedSolutionsCount = 0;
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

	if (outFile.is_open() && !fileData.empty())
		outFile << fileData;
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

void IOManager::GenerateKeyFile(const std::string fileName, int solutionCount)
{
	Number root;
	Number uHash;
	Number lHash;
	Number pHash;

	fileData += "NS " + std::to_string(solutionCount) + "\n";

	for (int i = 0; i < solutionCount; i++)
	{
		root.SetDigits(Number::GenerateRandomFourDigits(true));
		fileData += "ROOT " + Number::GetStringFromDigits(root.GetDigits()) + "\n";

		uHash.SetDigits(Number::GenerateRandomFourDigits());
		fileData += "UHF " + ToString(uHash) + "\n";

		lHash.SetDigits(Number::GenerateRandomFourDigits());
		fileData += "LHF " + ToString(lHash) + "\n";

		pHash.SetDigits(Number::GenerateRandomFourDigits());
		fileData += "PHF " + ToString(pHash) + "\n";
	}
	WriteToFile(fileName);
	ClearDataString();
}

void IOManager::ParseKeyFile(const std::string keyFileName)
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
			PrintToConsole("\nSomething went wrong while trying to parse the key file.", 1);
			break;
		}
		digitsVector.clear();
	}
	ClearDataString();
}

void IOManager::GenerateMultiSafeFileHashes()
{
	for (int i = 0; i < solutionCount; i++)
	{
		MultiLockSafe* safe = new MultiLockSafe(numberOfLocksPerSafe);
		safe->LockTheSafe(roots.at(i), uHashes.at(i), lHashes.at(i), pHashes.at(i));

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
/// => Ensure the sum of the digits on the combination lock to the left is less than the sum of the digits of the combination lock to the right
/// => Ensure that the sum of numbers across all locks is even
void IOManager::CheckMultiSafeFileValidity(std::vector<std::string> & validityList)
{
	std::string temp;
	int counter = -1;
	bool same = false;

	for (std::vector<Number>::iterator it = cns.begin(); it != cns.end(); it++)
	{
		// CNs must not have repeating digits
		if ((*it).GetDigits().at(0) == (*it).GetDigits().at(1) || (*it).GetDigits().at(0) == (*it).GetDigits().at(2) ||
			(*it).GetDigits().at(0) == (*it).GetDigits().at(3) || (*it).GetDigits().at(1) == (*it).GetDigits().at(2) ||
			(*it).GetDigits().at(1) == (*it).GetDigits().at(3) || (*it).GetDigits().at(2) == (*it).GetDigits().at(3))
			same = true;

		if (it - cns.begin() == counter + numberOfLocksPerSafe)
		{
			if (same)
				temp = NOT_VALID;
			else
				temp = VALID;
			validityList.push_back(temp);
			counter = it - cns.begin();
			same = false;
		}
	}
}

void IOManager::WriteMultiSafeFile(const std::string multiSafeFileName)
{
	GenerateMultiSafeFileHashes();
	CheckMultiSafeFileValidity(validityList);

	fileData += "NS " + std::to_string(solutionCount) + " ";
	std::string tempLockedSolutions; /// new function (cw2)

	for (int i = 0; i < solutionCount; i++)
	{
		/// TODO: try catch if validityList.size() != solutionCount
		fileData += validityList.at(i);

		/// new function (cw2)
		if (validityList.at(i) == VALID)
		{
			tempLockedSolutions += "\nROOT: " + Number::GetStringFromDigits(roots.at(i).GetDigits()) + "\n";
			lockedSolutionsCount++;
		}

		for (int j = 0; j < numberOfLocksPerSafe; j++)
		{
			fileData += "CN" + std::to_string(j) + " " + Number::GetStringFromDigits(cns.at(i * numberOfLocksPerSafe + j).GetDigits()) + ", ";
			fileData += "LN" + std::to_string(j) + " " + Number::GetStringFromDigits(lns.at(i * numberOfLocksPerSafe + j).GetDigits()) + ", ";
			fileData += "HN" + std::to_string(j) + " " + Number::GetStringFromDigits(hns.at(i * numberOfLocksPerSafe + j).GetDigits()) + ",\n";

			/// new function (cw2)
			if (validityList.at(i) == VALID)
			{
				tempLockedSolutions += "LN" + std::to_string(j) + ": " + Number::GetStringFromDigits(lns.at(i * numberOfLocksPerSafe + j).GetDigits()) + "\n";
			}
		}
	}
	/// new function (cw2)
	lockedFileData += "NL " + std::to_string(lockedSolutionsCount) + "\n";
	lockedFileData += tempLockedSolutions;

	WriteToFile(multiSafeFileName);
	ClearDataString();
}

void IOManager::GenerateMultiSafeFile(const std::string keyFileName, const std::string multiSafeFileName)
{
	ParseKeyFile(keyFileName);
	WriteMultiSafeFile(multiSafeFileName);
}

std::vector<int> IOManager::StringToIntegerVector(std::string string)
{
	const char* cstr = string.c_str();
	std::vector<int> digitsVector;
	std::stringstream parser;
	int digit;

	for (int i = 0; i < (int)string.size(); i++)
	{
		if (isdigit(cstr[i]))
		{
			if (cstr[i] == '0')
				digitsVector.push_back(0);
			else
			{
				parser << cstr[i];
				parser >> digit;
				parser.str(std::string());
				parser.clear();

				if (cstr[i - 1] == '-')
					digitsVector.push_back(-digit);
				else
					digitsVector.push_back(digit);
			}
		}
	}
	return digitsVector;
}

/// cw2
void IOManager::GenerateLockedSafeFile(const std::string lockedSafeFileName)
{
	fileData = lockedFileData;

	WriteToFile(lockedSafeFileName);
	lockedFileData = "";
	ClearDataString();
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
