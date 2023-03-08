#pragma once
#include <iostream>
#include "BackendCore.h"
#include "CBDutils.h"
#include "crow_all.h"
#include <vector>
class BackendCore;

class BackendRouter {
	std::vector<std::string> _avalibleOrigins;
	int _serverPort;
	BackendCore* _corePtr; //default pointer bacause core defined on stack
	crow::SimpleApp app;
	crow::response _baseResponse;
public:
	BackendRouter(BackendCore* corePtr,int serverPort = 8080, std::vector<std::string> avalibleOrigins = std::vector<std::string>());
	void initAllRoutes();
	void setOriginsArr(std::vector<std::string> avalibleOrigins);
	void setBaseResponseHeaders(std::vector<std::string> originsArr);
	void runServer();
};