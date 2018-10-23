#include "IOManager.h"

IOManager::IOManager()
{
	solutionCount = 0;
	lockedSolutionsCount = 0;
}

IOManager::~IOManager()
{
}

void IOManager::OpenFile(const std::string fileName, FileType type)
{
	if (type == READ_IN)
	{
		try
		{
			inFile.open(INPUT_DIRECTORY + fileName);
		}
		catch (const std::ifstream::failure& e)
		{
			throw std::ifstream::failure("An ifstream::failure error was caught while trying to open " + fileName + "\n" + "Explanatory string: " + e.what() + "\n");
		}
	}
	else if (type == WRITE_OUT)
	{
		try
		{
			outFile.open(OUTPUT_DIRECTORY + fileName);
		}
		catch (const std::ofstream::failure& e)
		{
			throw std::ofstream::failure("An ofstream::failure error was caught while trying to open " + fileName + "\n" + "Explanatory string: " + e.what() + "\n");
		}
	}
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
	ClearDataString(fileData);
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
	ClearDataString(fileData);
}

void IOManager::ParseLockedSafeFile(const std::string lockedSafeFileName)
{
	ReadFromFile(lockedSafeFileName);

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
	ClearDataString(fileData);
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
			if (lockedSolutionsCount == 0)
				tempLockedSolutions += "ROOT: " + Number::GetStringFromDigits(roots.at(i).GetDigits()) + "\n";
			else
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
	ClearDataString(fileData);
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
	ClearDataString(lockedFileData);
	ClearDataString(fileData);
}

/// TODO:
void IOManager::UnlockLockedSafeFile(const std::string lockedSafeFileName)
{
	// Use the locked_file.txt from someone else to unlock all its safes => Basically what CW 2 is about
	// Find out what the uHashes, lHashes and pHashes are to input in a function to generate the LN values, which in turn will reveal the CN values
	// and the you just PressButton(CNs) to unlock all the locks in a safe => all safes and finally get a 'Success' message.
	/// The key is something to do with the fact that the hashes for each safe are the same for all locks.

	/// TODO:
	/// 1. Read from a locked_file.txt [DONE]
	/// 2. Get the number of solutions from it and create a for loop with that much safes [DONE]
	/// 3. Try to unlock each one via the help of a user interface and some kind of algorithm to generate a safe's first lock's CN
	/// 4. Get candidate solutions (the first lock's CN) and narrow them to the smallest possible number of candidates

	ParseLockedSafeFile(lockedSafeFileName);

	for (int i = 0; i < solutionCount; i++)
	{
		MultiLockSafe* safe = new MultiLockSafe(numberOfLocksPerSafe); /// Add user input for this
		safe->LockTheSafe(GetRoots().at(0), GetUHashes().at(0), GetLHashes().at(0), GetPHashes().at(0));

		std::string temp = "\nWould you like to try and unlock safe " + i;
		temp += " out of " + solutionCount;
		temp += "? (y/n)";
		PrintToConsole(temp, 1);
		char ans;
		std::cin >> ans;
		if (ans == 'y')
			safe->UnlockTheSafe();

		delete safe;
	}
}

void IOManager::ChangeNumberOfLocksPerSafe()
{
	std::string temp = "\nWould you like to change the default number of locks per safe (the current value is: " + numberOfLocksPerSafe;
	temp += ")? (y/n)";
	PrintToConsole(temp, 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		PrintToConsole("\nPlease specify the number of locks per safe: ", 1);
		int numLocks;
		std::cin >> numLocks;
		while (!isdigit(numLocks))
		{
			PrintToConsole("\nYou haven't entered a correct number of locks. Try again: ", 1);
			std::cin >> numLocks;
		}
		numberOfLocksPerSafe = numLocks;
	}
}

void IOManager::GenerateKeyFileUI()
{
	PrintToConsole("\nWould you like to generate a key file? (y/n)", 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		std::string keyFileName;
		int solutionCount;

		PrintToConsole("\nEnter the name of the key file you wish to write to: ", 1);
		std::cin >> keyFileName;
		VerifyFileFormat(keyFileName);

		PrintToConsole("\nNow enter the number of solutions you wish to generate: ", 1);
		std::cin >> solutionCount;
		while (!isdigit(solutionCount))
		{
			PrintToConsole("\nYou haven't entered a correct number of solutions. Try again: ", 1);
			std::cin >> solutionCount;
		}
		GenerateKeyFile(keyFileName, solutionCount);
		PrintToConsole("\nKey file successfully created!", 1);
	}
}

void IOManager::VerifyFileFormat(std::string & fileName)
{
	std::string comparable;
	std::string tempFileName = fileName;

	for (int i = 0; i < 4; i++)
	{
		comparable += tempFileName.back();
		tempFileName.pop_back();
	}
	if (comparable != FILE_FORMAT)
		fileName += FILE_FORMAT;
}

//bool IOManager::IsSolutionCountInteger(const int solutionCount)
//{
//	bool isNumber = true;
//	std::vector<int> solutionCountString = StringToIntegerVector(ToString(solutionCount));
//
//	for (auto it = solutionCountString.begin(); it != solutionCountString.end(); it++)
//	{
//		if (!isdigit(*it))
//			isNumber = false;
//	}
//	return isNumber;
//}

void IOManager::GenerateMultiSafeFileUI()
{
	PrintToConsole("\nWould you like to generate a multi_safe file? (y/n)", 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		std::string keyFileName;
		std::string multiSafeFileName;

		PrintToConsole("\nEnter the name of the key file you wish to read in: ", 1);
		std::cin >> keyFileName;
		VerifyFileFormat(keyFileName);

		PrintToConsole("Enter the name of the multi_safe file you wish to write to: ", 1);
		std::cin >> multiSafeFileName;
		VerifyFileFormat(multiSafeFileName);

		GenerateMultiSafeFile(keyFileName, multiSafeFileName);
		PrintToConsole("\nMulti_safe file successfully created!", 1);
	}
}

void IOManager::GenerateLockedSafeFileUI()
{
	PrintToConsole("\nWould you like to generate a locked_safe file? (y/n)", 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		std::string lockedSafeFileName;

		PrintToConsole("\nEnter the name of the locked_safe file you wish to write to: ", 1);
		std::cin >> lockedSafeFileName;
		VerifyFileFormat(lockedSafeFileName);

		GenerateLockedSafeFile(lockedSafeFileName);
		PrintToConsole("\nLocked_safe file successfully created!", 1);
	}
}

void IOManager::UnlockSafesUI()
{
	PrintToConsole("Would you like to unlock a locked_safe.txt file? (y/n)", 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		std::string lockedSafeFileName;

		PrintToConsole("\nEnter the name of the locked_safe file you wish to read from: ", 1);
		std::cin >> lockedSafeFileName;
		VerifyFileFormat(lockedSafeFileName);

		UnlockLockedSafeFile(lockedSafeFileName);
		PrintToConsole("\nLocked_safe file successfully unlocked!", 1);
	}
}

void IOManager::ClearDataString(std::string & dataString)
{
	dataString = "";
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
