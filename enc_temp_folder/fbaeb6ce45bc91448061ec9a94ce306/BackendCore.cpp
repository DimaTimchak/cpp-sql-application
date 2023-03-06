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

	std::cout <<"\nIn handle: " << requestData<<"\n\n";
	//_backendDataBase->createEntity()
}
