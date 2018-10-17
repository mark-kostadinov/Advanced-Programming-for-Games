#include <time.h>
#include "MultiLockSafe.h"

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand(time(0));

	MultiLockSafe* test = new MultiLockSafe(1);
	delete test;

	int exit;
	PrintToConsole("Please enter a key to exit...", 1);
	std::cin >> exit;

	return 0;
}