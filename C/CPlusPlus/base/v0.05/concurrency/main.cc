#include <iostream>
#include "Thread.h"

int main(int argc, char** argv)
{
	Thread* thread = new Thread();
	thread->run();
	thread->join();
	return 0;
}
