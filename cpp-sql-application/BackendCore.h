#pragma once
#include "BackendDataBase.h"
#include "BackendRouter.h"
#include "crow_all.h"
#include "CBDutils.h"
#include <memory>
class BackendRouter;

class BackendCore{
	
	std::unique_ptr<BackendRouter> _backendRouter;
	std::unique_ptr<BackendDataBase> _backendDataBase;
	std::string _configFilePath;
	int _serverPort;
public:
	BackendCore(std::unique_ptr<BackendDataBase> bdb
	,std::string configFilePath = "conf/user-database-conf.json"
	, int serverPort = 8080);
	void launchBackends();
	void handleAddEntity(std::string requestData);
	std::string handleReadTableAttr(std::string tableName, int page = 1);
};