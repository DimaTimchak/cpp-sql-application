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

	std::vector<std::string> parsedValuesArr;
	for (int i = 0; i < valuesArr.size(); i++) {
		if (para.second.substr(0,8) == "https://" && valuesArr[i].substr(0, 8) != "https://") {
			
			parsedValuesArr.push_back("https://"+valuesArr[i]);
		}
		else if (para.second.substr(0, 7) == "http://" && valuesArr[i].substr(0, 7) != "http://")
		{
			parsedValuesArr.push_back("http://" + valuesArr[i]);
		}
		else {
			parsedValuesArr.push_back(valuesArr[i]);
		}
	}
	for (int i = 0; i < parsedValuesArr.size(); i++) {
		if (parsedValuesArr[i] == para.second) {
			return i;
		}
	}
	
	return valuesArr.size();	
}
std::string CBDutils::GetStringBeforeSpace(std::string data)
{
	std::string temp;
	temp = data.substr(0, data.find(" "));
	return temp;
}
