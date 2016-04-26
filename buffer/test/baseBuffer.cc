#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "baseBuffer.h"

int main(int argc, char** argv)
{
	int fd = open("./main.cc", O_RDONLY);
	int fd1 = open("out.txt", O_CREAT|O_WRONLY);
    baseBuffer buf;
	return 0;
}
