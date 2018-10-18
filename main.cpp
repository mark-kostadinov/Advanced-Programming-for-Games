#include <time.h>
#include "MultiLockSafe.h"

/// Add file I/O

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand((unsigned int)time(0));

	MultiLockSafe* test = new MultiLockSafe(2);
	delete test;

	/// DEBUG - TODO: PrintToConsoleFormatted()
	for (int i = 0; i < 100; i++)
	{
		std::vector<int> v = GenerateRandomFourDigits();
		for (int j = 0; j < 4; j++)
		{
			if (v.at(j) >= 0)
				PrintToConsole("  ");
			else
				PrintToConsole(" ");
			PrintToConsole(v.at(j));
			PrintToConsole(",");
		}
		PrintToConsole("", 1);
	}

	int exit;
	PrintToConsole("\nPlease enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}