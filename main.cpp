#include <iostream>
#include <time.h>
#include "MultiLockSafe.h"

int main()
{
	// Initialize the pseudo-random generator to a distinctive runtime value
	srand(time(0));

	MultiLockSafe* test = new MultiLockSafe(1);

	delete test;

	return 0;
}