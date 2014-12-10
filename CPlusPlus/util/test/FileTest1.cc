#include <iostream>
#include "File.h"

using namespace NetworkLib::Util;
int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
	}

	File file(argv[1]);
	file.init();
	if (file.canRead()) {
		std::cout << "ReadAble" << std::endl;
	}

	if (file.canWrite()) {
		std::cout << "WriteAble" << std::endl;
	}
	return 0;
}
