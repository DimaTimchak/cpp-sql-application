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

std::string BackendCore::handleReadTableAttr(std::string tableName, int page)
{
	return std::string(_backendDataBase->readEntity(tableName, page));
 }

void BackendCore::handleDeletEntity(std::string table, std::string entityId)
{
	_backendDataBase->deleteEntity(table, entityId);
}

void BackendCore::handleUpdateEntity(std::string requestData)
{
	std::string tableName, value = "", id;
	crow::json::wvalue parsedTemp = (crow::json::load(requestData));
	std::vector <std::string> parsedAtribute, parsedValue, NotNull;

	tableName = CBDutils::cutQuotes(parsedTemp[0].dump());

	for (int i = 1; i < parsedTemp.size(); i++)
	{
		if (std::string(parsedTemp[i].dump()) != "null") NotNull.push_back(parsedTemp[i].dump());
	}

	id = CBDutils::GetStringBeforeSpace(crow::json::load(NotNull[0])["what"].s());  // Get "ID-value"

	for (int i = 0; i < NotNull.size(); i++)
	{
		parsedAtribute.push_back(CBDutils::GetStringBeforeSpace(crow::json::load(NotNull[i])["who"].s()));
		parsedValue.push_back(CBDutils::GetStringBeforeSpace(crow::json::load(NotNull[i])["what"].s()));
	}

	parsedAtribute.erase(parsedAtribute.begin());	//Delete "ID"
	parsedValue.erase(parsedValue.begin());			//Delete "ID-value"

	for(int i = 0; i < parsedAtribute.size(); i++ )
	{
			if (i != parsedAtribute.size() - 1)
			{
				value += parsedAtribute[i] + " = " + "\"" + parsedValue[i] + "\"" + ", ";
			}
			else
				value += parsedAtribute[i] + " = " + "\"" + parsedValue[i] +"\"";
	}
	
	_backendDataBase->updateEntity(tableName, value, id);
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
