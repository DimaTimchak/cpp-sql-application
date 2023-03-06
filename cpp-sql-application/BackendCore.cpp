#include "BackendCore.h"

BackendCore::BackendCore(std::unique_ptr<BackendDataBase> bdb
	,std::string configFilePath
	,int serverPort)
	:_backendDataBase(std::move(bdb))
	,_configFilePath(configFilePath)
	,_serverPort(serverPort)
{
	_backendDataBase->setPath(configFilePath); // not as elegant as it could be :(


	crow::json::wvalue approvedOrigins = crow::json::load(CBDutils::loadConfig(configFilePath))["approvedOrigins"];
	std::vector<std::string> approvedOriginsVec;
	for (int i = 0; i < approvedOrigins.size(); i++) {
		approvedOriginsVec.push_back( CBDutils::cutQuotes(approvedOrigins[i].dump()));
	}
	_backendRouter = (std::make_unique<BackendRouter>(this, serverPort,approvedOriginsVec));
}
void BackendCore::launchBackends()
{
	_backendDataBase->initDataBase();
	_backendRouter->runServer();
	
}

void BackendCore::handleAddEntity(std::string requestData)
{	
	crow::json::wvalue parsedTemp= (crow::json::load(requestData));
	std::vector <std::string> NotNull, parsedKey;
	std::vector <std::string> parsedvalue;
	std::string value = "", key = "", TableName;
	for (int i = 1; i < parsedTemp.size(); i++)
	{
		if (std::string(parsedTemp[i].dump()) != "null") NotNull.push_back (parsedTemp[i].dump());
	}
	TableName = parsedTemp[0].dump();
	
	for (int i = 0; i < NotNull.size(); i++)
	{
		parsedKey.push_back(crow::json::load(NotNull[i])["who"].s());
		parsedvalue.push_back(crow::json::load(NotNull[i])["what"].s());
	}

	for (int i = 0; i < parsedKey.size(); i++)
	{
		if (parsedKey.size() - 1 != i) key += CBDutils::GetStringBeforeSpace(parsedKey[i]) + ", ";
		else
			key += CBDutils::GetStringBeforeSpace(parsedKey[i]);
		if (parsedvalue.size() - 1 != i) value += "'" + CBDutils::GetStringBeforeSpace(parsedvalue[i]) + "', ";
		else
			value += "'" + CBDutils::GetStringBeforeSpace(parsedvalue[i]) + "'";
	}
	_backendDataBase->createEntity(key,value,CBDutils::cutQuotes(TableName));
}
