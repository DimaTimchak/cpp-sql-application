#include "SQLDataBase.h"

std::string SQLDataBase::_loadConfig()
{
	std::ifstream file(_configureFilePath);
	//file.unsetf(std::ios_base::skipws);
	std::ostringstream tmp;
	std::string confInternals = "";
	std::string sym;

	if (file) {
		for (file >> sym; !file.eof(); file >> sym) {
			tmp << file.rdbuf();
		}
		confInternals = tmp.str();
	}
	else {
		throw std::invalid_argument(_configureFilePath + " file not found");
	}
	std::cout << confInternals;
	return confInternals;
}

std::vector<std::vector<std::string>> SQLDataBase::_parseJsonTable(crow::json::wvalue dbTableInternals)
{
	std::vector<std::vector<std::string>> tempTableArr(dbTableInternals.size());
	
		
		for (int i = 0; i < dbTableInternals.size(); i++) {
			for (int j = 0; j < crow::json::load(dbTableInternals[i].dump()).size(); j++) {
				// convert raw string to json parser then divides it into parts 
				// and assigns them to the corresponding elements of the array
				tempTableArr[i].push_back((crow::json::load(dbTableInternals[i].dump())[j]).s());

			}
		}
	
	
	
	return tempTableArr;
}

SQLDataBase::SQLDataBase(std::string configureFilePath):BackendDataBase(configureFilePath){
}


int SQLDataBase::initDataBase() {
	crow::json::wvalue jsonConfInternals;
	try{
	jsonConfInternals = crow::json::load(_loadConfig());
	}
	catch (std::exception& e) {
		throw std::runtime_error(std::string("Template table is filled in incorrectly: ") + e.what());
	}

//	std::vector<std::string>keys= jsonConfInternals.keys();

	_dbName = jsonConfInternals["dbName"].dump() != "null" //TODO (refactor): move to the corresponding "initConfigParams" function
		? jsonConfInternals["dbName"].dump().substr(1, jsonConfInternals["dbName"].dump().size()-2)
		: "undefined";

	_dbHost = jsonConfInternals["dbHost"].dump() != "null"
		? jsonConfInternals["dbHost"].dump().substr(1, jsonConfInternals["dbHost"].dump().size() - 2)
		: "undefined";
	

	_dbPort =  jsonConfInternals["dbPort"].dump() != "null"
		? stoi(jsonConfInternals["dbPort"].dump())
		: 0;
	
	_dbUser = jsonConfInternals["dbUser"].dump() != "null"
		? jsonConfInternals["dbUser"].dump().substr(1, jsonConfInternals["dbUser"].dump().size() - 2)
		: "undefined";

	_dbPassword = jsonConfInternals["dbPassword"].dump() != "null"
		? jsonConfInternals["dbPassword"].dump().substr(1, jsonConfInternals["dbPassword"].dump().size() - 2)
		: "undefined";
	if (_dbName == "undefined" || _dbHost == "undefined" || _dbPort == 0 || _dbUser == "undefined" || _dbPassword == "undefined") {
		throw std::invalid_argument("invalid config params");
	}

	_dbTables = _parseJsonTable(crow::json::load(jsonConfInternals["dbTables"].dump()));

	try{
	 _sqlSession =std::make_unique<mysqlx::Session>(mysqlx::SessionOption::HOST, _dbHost,
		 mysqlx::SessionOption::PORT, _dbPort,
		 mysqlx::SessionOption::USER, _dbUser,
		 mysqlx::SessionOption::PWD, _dbPassword);
	}
	catch (mysqlx::abi2::r0::Error){
		throw std::invalid_argument("cannot connect to Data base, chek your internet conections and correctness of the configuration file!");
	}
	//for (int i = 0; i < _dbTables.size(); i++) {
	//	_sqlSession->sql(std::string("CREATE DATABASE ")+ _dbTables[i][0]).execute();
	//}

	return 0;
}

SQLDataBase::~SQLDataBase()
{
}
