#include <time.h>
#include "IOManager.h"

/// 1. Should I stick to the stack allocation approach or switch to heap-based? => I think I will need some heap allocation for runtime checks 
/// of the safes during the second part of the coursework.
/// 2. Add file headers (author, description, etc.)

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

	// Pretty 1-dimensional but does the job
	io.ChangeNumberOfLocksPerSafe();
	io.GenerateKeyFileUI();
	io.GenerateMultiSafeFileUI();
	io.GenerateLockedSafeFileUI();
	io.UnlockSafesUI();

	int exit;
	PrintToConsole("\nPlease enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}