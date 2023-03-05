#pragma once
#include "BackendDataBase.h"
#include "BackendRouter .h"
#include "crow_all.h"
#include "CBDutils.h"
class BackendCore{
	std::string configFilePath;
	/*std::unique_ptr<BackendRouter> backendRouter;
	std::unique_ptr<BackendDataBase> backendDataBase;*/
public:
	BackendCore();

};