#pragma once
#include "BackendDataBase.h"
#include "crow_all.h"
#include <memory>
#include <mysqlx/xdevapi.h>
#include <fstream>
class SQLDataBase : public BackendDataBase 
{
	
	std::unique_ptr<mysqlx::Session>_sqlSession;
	std::vector<std::vector<std::string>>_dbTables;
	/*Loads configuration data from JSON file*/
	std::string _loadConfig();
	std::vector<std::vector<std::string>> _parseJsonTable(crow::json::wvalue dbTableInternals);
	std::string _getConfParam(std::vector<std::string> confParamString);
	
public:
	SQLDataBase(std::string configureFilePath = "conf/user-database-conf.json");
	 int initDataBase () override;
	 bool  isDatabaseExist() override;
	 void setPath(std::string configureFilePath) override;
	 std::vector<int> isTableExist();
	 void createEntity(std::string atribute, std::string value, std:: string TableName) override;
	 std::string readEntity(std::string TableName, int page) override;
	 std::string updateEntity() override;
	 std::string deleteEntity() override;
};