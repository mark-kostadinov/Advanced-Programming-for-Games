#include "IOManager.h"

int numberOfLocksPerSafe = 5;
int lockedLNcounter = 0;

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
		inFile.open(FILE_IO_DIRECTORY + fileName);
		if (!inFile || inFile.fail())
		{
			throw std::ifstream::failure("An ifstream::failure error was caught while trying to open " + fileName + "\n");
			exit(1);
		}
	}
	else if (type == WRITE_OUT)
	{
		outFile.open(FILE_IO_DIRECTORY + fileName);
		if (!outFile || outFile.fail())
		{
			throw std::ofstream::failure("An ofstream::failure error was caught while trying to open " + fileName + "\n");
			exit(1);
		}
	}

	/// TODO: Catch block never executes => the file which is trying to execute on does not exist => the program crashes
	//	try
	//	{
	//		inFile.open(FILE_IO_DIRECTORY + fileName);
	//	}
	//	catch (const std::ifstream::failure& e)
	//	{
	//		throw std::ifstream::failure("An ifstream::failure error was caught while trying to open " + fileName + "\n" + "Explanatory string: " + e.what() + "\n");
	// exit(1);
	//	}
	//}
	//else if (type == WRITE_OUT)
	//{
	//	try
	//	{
	//		outFile.open(FILE_IO_DIRECTORY + fileName);
	//	}
	//	catch (const std::ofstream::failure& e)
	//	{
	//		throw std::ofstream::failure("An ofstream::failure error was caught while trying to open " + fileName + "\n" + "Explanatory string: " + e.what() + "\n");
	// exit(1);
	//	}
	//}
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
		{
			if (line == "") continue; // Skip blank line
			fileData += line + "\n";
		}
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
	std::vector<int> digitsVector;
	SplitData(lines, fileData);
	ParseSolutionCount(lines, digitsVector); // Get just the first line which contains the number of solutions in the file

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
	std::vector<int> digitsVector;
	SplitData(lines, fileData);
	ParseSolutionCount(lines, digitsVector); // Get just the first line which contains the number of solutions in the file

	// Then continue with the rest of the file
	Number num;
	int counter = 0;
	int k = 0;

	for (std::list<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		digitsVector = StringToIntegerVector((*it).substr((*it).length() - 6, (*it).length()));
		num.SetDigits(digitsVector);

		switch (counter)
		{
		case 0:
			roots.push_back(num);
			counter++;
			break;
		case 1:
			lns.push_back(num);
			if (k < numberOfLocksPerSafe)
				k++;
			else
			{
				k = 0;
				counter = 0;
			}
			break;
		default:
			PrintToConsole("\nSomething went wrong while trying to parse the locked_safe file.", 1);
			break;
		}
		digitsVector.clear();
	}
	ClearDataString(fileData);
}

void IOManager::ParseSolutionCount(std::list<std::string>& lines, std::vector<int>& digitsVector)
{
	std::string solutionCountString = lines.front();
	digitsVector = StringToIntegerVector(solutionCountString);
	solutionCount = Number::GetIntegerFromDigits(digitsVector);
	digitsVector.clear();
	lines.pop_front();
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

// Use the generated multi-safe file to identify if each entry is or is not a valid output given the requirements:
/// TODO: Ensure the sum of the digits on the combination lock to the left is less than the sum of the digits of the combination lock to the right
/// TODO: Ensure that the sum of numbers across all locks is even
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
	std::string tempLockedSolutions;

	for (int i = 0; i < solutionCount; i++)
	{
		try
		{
			fileData += validityList.at(i);
		}
		catch (const std::out_of_range& oor)
		{
			std::string exceptionString = "Out of range exception caught: ";
			exceptionString += oor.what();
			PrintToConsole(exceptionString, 1);
			exit(1);
		}

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

			if (validityList.at(i) == VALID)
				tempLockedSolutions += "LN" + std::to_string(j) + ": " + Number::GetStringFromDigits(lns.at(i * numberOfLocksPerSafe + j).GetDigits()) + "\n";
		}
	}
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

void IOManager::GenerateLockedSafeFile(const std::string lockedSafeFileName)
{
	fileData = lockedFileData;

	WriteToFile(lockedSafeFileName);
	ClearDataString(lockedFileData);
	ClearDataString(fileData);
}

void IOManager::UnlockLockedSafeFile(const std::string lockedSafeFileName)
{
	ClearDataStructures();
	ParseLockedSafeFile(lockedSafeFileName);
	UnlockUsingRNG();
}

void IOManager::ChangeNumberOfLocksPerSafe()
{
	std::string temp = "Would you like to change the default number of locks per safe (the current value is: " + std::to_string(numberOfLocksPerSafe);
	temp += ")? (y/n)";
	PrintToConsole(temp, 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
	{
		PrintToConsole("\nPlease specify the number of locks per safe: ", 1);
		int numLocks;
		while (!(std::cin >> numLocks) || numLocks < 0)
		{
			PrintToConsole("\nYou didn't enter a correct number of locks. Try again: ", 1);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
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
		while (!(std::cin >> solutionCount) || solutionCount < 0)
		{
			PrintToConsole("\nYou haven't entered a correct number of solutions. Try again: ", 1);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
		GenerateKeyFile(keyFileName, solutionCount);
		PrintToConsole("\nKey file successfully created!", 1);
	}
}

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

		PrintToConsole("\nEnter the name of the multi_safe file you wish to write to: ", 1);
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
	PrintToConsole("\nWould you like to unlock a locked_safe file? (y/n)", 1);
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

		//=====================================

		std::string keyFileName;

		PrintToConsole("\nEnter the name of the key file you wish to write to: ", 1);
		std::cin >> keyFileName;
		VerifyFileFormat(keyFileName);

		GenerateKeyFileFromLockedFile(keyFileName);
		PrintToConsole("\nKey file from locked file successfully created!", 1);

		//======================================

		std::string multiSafeFileName;

		PrintToConsole("\nEnter the name of the multi_safe file you wish to write to: ", 1);
		std::cin >> multiSafeFileName;
		VerifyFileFormat(multiSafeFileName);

		GenerateMultiSafeFile(keyFileName, multiSafeFileName);
		PrintToConsole("\nMulti_safe file from locked file successfully created!", 1);
	}
}

void IOManager::VerifyFileFormat(std::string & fileName)
{
	if (!fileName.empty())
	{
		std::string lastChar = std::to_string(fileName.back());
		std::string currentFileFormat;

		currentFileFormat = GetLastThreeCharsFromString(fileName);

		if (fileName.find('.') != fileName.find_last_of('.') && fileName.find('.') != std::string::npos)
		{
			fileName.erase(fileName.find('.'));

			if (lastChar == std::to_string('.'))
				fileName.push_back('.');

			currentFileFormat = GetLastThreeCharsFromString(fileName);
		}

		if (currentFileFormat != FILE_FORMAT && fileName.at(fileName.length() - 1) != '.')
			fileName += DOT_FILE_FORMAT;
		else if (currentFileFormat != FILE_FORMAT)
			fileName += FILE_FORMAT;
	}
}

std::string IOManager::GetLastThreeCharsFromString(const std::string & string)
{
	std::string tempFileName = string;
	std::string result;

	for (int i = 0; i < 3; i++)
	{
		if (!tempFileName.empty())
		{
			result += tempFileName.back();
			tempFileName.pop_back();
		}
	}
	return result;
}

void IOManager::ClearDataString(std::string & dataString)
{
	dataString = "";
}

void IOManager::ClearDataStructures()
{
	roots.clear();
	uHashes.clear();
	lHashes.clear();
	pHashes.clear();
	cns.clear();
	lns.clear();
	hns.clear();
	validityList.clear();
}

void IOManager::UnlockUsingRNG()
{
	PrintToConsole("\nDecyphering...", 1);

	for (int i = 0; i < solutionCount; i++)
	{
		int j = 0;
		bool finished = false;
		MultiLockSafe* safe = new MultiLockSafe(numberOfLocksPerSafe);
		std::vector<Number> unlockedLNs;

		while (!finished)
		{
			/// TODO: Add time counter
		//again:
			Number temp;
			temp.SetDigits(Number::GenerateRandomFourDigits());
			lockedUHashes.push_back(temp);
			temp.SetDigits(Number::GenerateRandomFourDigits());
			lockedLHashes.push_back(temp);
			temp.SetDigits(Number::GenerateRandomFourDigits());
			lockedPHashes.push_back(temp);

			safe->LockTheSafe(roots.at(i), lockedUHashes.at(j), lockedLHashes.at(j), lockedPHashes.at(j));

			/// Method to discard all invalid CNs => more checks, runs slower tho
			//if (lockedCNs.size() > 0)
			//	lockedCNs.clear();
			//for (int k = 0; k < numberOfLocksPerSafe; k++)
			//	lockedCNs.push_back(safe->GetCombinationLocksVector().at(k)->GetCN());

			//for (std::vector<Number>::iterator it = lockedCNs.begin(); it != lockedCNs.end(); it++)
			//{
			//	// CNs must not have repeating digits
			//	if ((*it).GetDigits().at(0) == (*it).GetDigits().at(1) || (*it).GetDigits().at(0) == (*it).GetDigits().at(2) ||
			//		(*it).GetDigits().at(0) == (*it).GetDigits().at(3) || (*it).GetDigits().at(1) == (*it).GetDigits().at(2) ||
			//		(*it).GetDigits().at(1) == (*it).GetDigits().at(3) || (*it).GetDigits().at(2) == (*it).GetDigits().at(3))
			//	{
			//		PrintToConsole("LockedCN at fault: " + Number::GetStringFromDigits((*it).GetDigits()), 1);
			//		lockedCNs.clear();
			//		lockedUHashes.pop_back();
			//		lockedLHashes.pop_back();
			//		lockedPHashes.pop_back();
			//		safe->UnlockAllLocks();
			//		goto again;
			//	}
			//}

			safe->UnlockTheSafe(lns, unlockedLNs);
			/// When it unlocks for the first time, capture the hashes and use them afterwards

			j++;

			// If the safe is unlocked by the UnlockTheSafe(), we're done
			if (!safe->IsLocked())
				finished = true;
			else // Unlock all the locks inside it, so they could be locked/hashed again with new values in the next iteration
				safe->UnlockAllLocks();
		}
		for (int k = 0; k < numberOfLocksPerSafe; k++)
			cns.push_back(safe->GetCombinationLocksVector().at(k)->GetCN());
		/// TODO: The hashes are cleared and the ones that are printed in the file are not the real ones => How to actually get them??
		uHashes.push_back(lockedUHashes.at(j - 1));
		lHashes.push_back(lockedLHashes.at(j - 1));
		pHashes.push_back(lockedPHashes.at(j - 1));

		GetLockedUHashes().clear();
		GetLockedLHashes().clear();
		GetLockedPHashes().clear();
		unlockedLNs.clear();

		delete safe;
		safe = NULL;
	}
	lockedLNcounter = 0;
}

void IOManager::GenerateKeyFileFromLockedFile(const std::string fileName)
{
	fileData += "NS " + std::to_string(solutionCount) + "\n";

	for (int i = 0; i < solutionCount; i++)
	{
		fileData += "ROOT " + Number::GetStringFromDigits(roots.at(i).GetDigits()) + "\n";
		fileData += "UHF " + ToString(uHashes.at(i)) + "\n";
		fileData += "LHF " + ToString(lHashes.at(i)) + "\n";
		fileData += "PHF " + ToString(pHashes.at(i)) + "\n";
	}
	WriteToFile(fileName);
	ClearDataString(fileData);
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
