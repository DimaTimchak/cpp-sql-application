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

std::string CBDutils::cutQuotes(std::string inputMessage)
{
	return inputMessage.substr(1, inputMessage.size() - 2);
}

int CBDutils::findCorespondingIndexInAppFromValueInMap(std::vector<std::string> valuesArr, crow::ci_map _map, std::string desiredKey)
{
	const std::pair<const std::string, std::string> para = *_map.find(desiredKey);
	std::cout << para.second;

	return 0;
}