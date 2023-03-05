#pragma once
#include <iostream>
#include "BackendCore.h"

#include <vector>
class BackendRouter {
	std::vector<std::string> _avalibleOrigins;
	int _serverPort;
	crow::SimpleApp app;
public:
	BackendRouter();
	BackendRouter(int serverPort);
	void initAllRoutes();
	void runServer();
};