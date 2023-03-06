#pragma once
#include <iostream>
#include <fstream>
#include <sstream>     
#include "crow_all.h"
namespace CBDutils {
	std::string loadConfig(std::string configureFilePath);
	std::string cutQuotes(std::string inputMessage);
	//if ci_map have value at desired key that exists in valuesArr returns coresponding arr index
	int findCorespondingIndexInAppFromValueInMap(std::vector<std::string> valuesArr,crow::ci_map _map,std::string desiredKey);

}