#include <time.h>
#include "MultiLockSafe.h"

/// 1. Fix digit generation (i.e. add negative option, etc.)
/// 2. Add file I/O

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand((unsigned int)time(0));

	MultiLockSafe* test = new MultiLockSafe(2);
	delete test;

	int exit;
	PrintToConsole("Please enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}