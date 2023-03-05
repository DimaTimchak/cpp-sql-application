#include "CBDutils.h"

std::string CBDutils::loadConfig(std::string configureFilePath)
{
	std::ifstream file(configureFilePath);
	//file.unsetf(std::ios_base::skipws);
	std::ostringstream tmp;
	std::string confInternals = "";
	std::string sym;

	if (file) {
		for (; !file.eof(); file >> sym) {
			tmp << file.rdbuf();
		}
		confInternals = tmp.str();
	}
	else {
		throw std::invalid_argument(configureFilePath + " file not found");
	}
	return confInternals;
}
