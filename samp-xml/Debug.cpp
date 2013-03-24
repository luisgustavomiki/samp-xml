#include "Debug.h"

void Debug::write(std::string s) {
	std::ofstream file;
	file.open ("xmldebug.txt", std::ios::app);
	file << s << std::endl;
	file.close();
}