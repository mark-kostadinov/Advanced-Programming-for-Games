#include <time.h>
#include "MultiLockSafe.h"

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand((unsigned int)time(0));

	MultiLockSafe* test = new MultiLockSafe(2);
	delete test;

	IOManager io;

	/// DEBUG - TODO: PrintToConsoleFormatted()
	for (int i = 0; i < 100; i++)
	{
		Number v;
		for (int j = 0; j < 4; j++)
		{
			if (v.GetDigits().at(j) >= 0)
				PrintToConsole("  ");
			else
				PrintToConsole(" ");
			PrintToConsole(v.GetDigits().at(j));
			PrintToConsole(",");
		}
		PrintToConsole("", 1);
	}

	/// TODO: Convert into a function and add checks
	std::string fileName;
	int solutionCount;
	PrintToConsole("\nEnter the name of the key file you wish to read in: ", 1);
	std::cin >> fileName;
	PrintToConsole("\nNow enter the number of solutions you wish to generate: ", 1);
	std::cin >> solutionCount;
	///if (fileName) doesn't have .txt => add it
	io.GenerateKeyFile(fileName, solutionCount);
	//io.GenerateKeyFile("key_file.txt", 100);
	/// if all is OK =>
	PrintToConsole("\nKey file successfully created!", 1);

	int exit;
	PrintToConsole("\nPlease enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}