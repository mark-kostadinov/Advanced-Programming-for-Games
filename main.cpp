#include <time.h>
#include "IOManager.h"

/// 1. Add exception checks for file I/O
/// 2. Should I stick to the stack allocation approach or switch to heap-based? => I thikn I will need some heap allocation for runtime checks 
/// of the safes during the second part of the coursework.
/// 3. Add file headers (author, description, etc.)

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand((unsigned int)time(0));

	IOManager io;

	/// DEBUG - TODO: PrintToConsoleFormatted()
	//for (int i = 0; i < 100; i++)
	//{
	//	Number v;
	//	for (int j = 0; j < 4; j++)
	//	{
	//		if (v.GetDigits().at(j) >= 0)
	//			PrintToConsole("  ");
	//		else
	//			PrintToConsole(" ");
	//		PrintToConsole(v.GetDigits().at(j));
	//		PrintToConsole(",");
	//	}
	//	PrintToConsole("", 1);
	//}

	/// TODO: Convert into a function and add checks
	//std::string fileName;
	//int solutionCount;
	//PrintToConsole("\nEnter the name of the key file you wish to read in: ", 1);
	//std::cin >> fileName;
	//PrintToConsole("\nNow enter the number of solutions you wish to generate: ", 1);
	//std::cin >> solutionCount;
	/////if (fileName) doesn't have .txt => add it
	//io.GenerateKeyFile(fileName, solutionCount);
	///// if all is OK =>
	//PrintToConsole("\nKey file successfully created!", 1);

	//io.GenerateKeyFile("key_file.txt", 100);
	io.GenerateMultiSafeFile("key_file.txt", "multi_safe_file.txt");
	io.GenerateLockedSafeFile("locked_safe_file.txt");

	/// TODO:
	MultiLockSafe* safe = new MultiLockSafe(numberOfLocksPerSafe);
	safe->LockTheSafe(io.GetRoots().at(0), io.GetUHashes().at(0), io.GetLHashes().at(0), io.GetPHashes().at(0));

	PrintToConsole("\nWould you like to try and unlock the safe? (y/n)", 1);
	char ans;
	std::cin >> ans;
	if (ans == 'y')
		safe->UnlockTheSafe();

	delete safe;

	int exit;
	PrintToConsole("\nPlease enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}