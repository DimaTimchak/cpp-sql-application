#include "SQLDataBase.h"


std::string SQLDataBase::_loadConfig()
{
	std::ifstream file(_configureFilePath);
	//file.unsetf(std::ios_base::skipws);
	std::ostringstream tmp;
	std::string confInternals = "";
	std::string sym;

	if (file) {
		for (; !file.eof(); file >> sym) {
			tmp << file.rdbuf();
		}
		confInternals = tmp.str();
	}
	else {
		throw std::invalid_argument(_configureFilePath + " file not found");
	}
	return confInternals;
}
std::vector<std::vector<std::string>> SQLDataBase::_parseJsonTable(crow::json::wvalue dbTableInternals)
{
	std::vector<std::vector<std::string>> tempTableArr (dbTableInternals.size());

	for (int i = 0; i < dbTableInternals.size(); i++) {
		for (int j = 0; j < crow::json::load(dbTableInternals[i].dump()).size(); j++) {
			// convert raw string to json parser then divides it into parts 
			// and assigns them to the corresponding elements of the array
			tempTableArr[i].push_back(std::string((crow::json::load(dbTableInternals[i].dump())[j]).s()));

		}
	}
	return tempTableArr;
}

void SQLDataBase::setPath(std::string configureFilePath) {
	_configureFilePath = configureFilePath;
}

std::string SQLDataBase::_getConfParam(std::vector<std::string>confParamString)
{
	std::string resultParam = "";
	for (int i=1; i < confParamString.size(); i++)
	{
		resultParam += confParamString[i];
		if (i != confParamString.size() - 1)
			resultParam += ", ";
	}
	return resultParam;
}

SQLDataBase::SQLDataBase(std::string configureFilePath) :BackendDataBase(configureFilePath) {
}


int SQLDataBase::initDataBase() {
	crow::json::wvalue jsonConfInternals;
	try {
		jsonConfInternals = crow::json::load(_loadConfig());
	}
	catch (std::exception& e) {
		throw std::runtime_error(std::string("Template table is filled in incorrectly: ") + e.what());
	}

	_dbName = jsonConfInternals["dbName"].dump() != "null" //TODO (refactor): move to the corresponding "initConfigParams" function
		? jsonConfInternals["dbName"].dump().substr(1, jsonConfInternals["dbName"].dump().size() - 2)
		: "undefined";

	_dbHost = jsonConfInternals["dbHost"].dump() != "null"
		? jsonConfInternals["dbHost"].dump().substr(1, jsonConfInternals["dbHost"].dump().size() - 2)
		: "undefined";


	_dbPort = jsonConfInternals["dbPort"].dump() != "null"
		? stoi(jsonConfInternals["dbPort"].dump())
		: 0;

	_dbUser = jsonConfInternals["dbUser"].dump() != "null"
		? jsonConfInternals["dbUser"].dump().substr(1, jsonConfInternals["dbUser"].dump().size() - 2)
		: "undefined";

	_dbPassword = jsonConfInternals["dbPassword"].dump() != "null"
		? jsonConfInternals["dbPassword"].dump().substr(1, jsonConfInternals["dbPassword"].dump().size() - 2)
		: "undefined";
	_dbSchemaName = jsonConfInternals["dbSchemaName"].dump() != "null"
		? jsonConfInternals["dbSchemaName"].dump().substr(1, jsonConfInternals["dbSchemaName"].dump().size() - 2)
		: "undefined";

	if (_dbName == "undefined" || _dbHost == "undefined" || _dbPort == 0 || _dbUser == "undefined" || _dbPassword == "undefined" || _dbSchemaName == "undefined") {
		throw std::invalid_argument("invalid config params");
	}

	_dbTables = _parseJsonTable(crow::json::load(jsonConfInternals["dbTables"].dump()));

	try
	{
		_sqlSession = std::make_unique<mysqlx::Session>(mysqlx::SessionOption::HOST, _dbHost,
			mysqlx::SessionOption::PORT, _dbPort,
			mysqlx::SessionOption::USER, _dbUser,
			mysqlx::SessionOption::PWD, _dbPassword);
	}
	catch (mysqlx::abi2::r0::Error)
	{
		throw std::invalid_argument("cannot connect to Data base, chek your internet conections and correctness of the configuration file!");
	}

	
	if (!isDatabaseExist()) _sqlSession->sql(std::string("CREATE DATABASE ") + _dbSchemaName).execute();
	_sqlSession->sql(std::string("USE ")+ _dbSchemaName).execute();
	std::vector<int> createIndex = isTableExist();
	for (int i = 0; i < createIndex.size(); i++)
	{
		_sqlSession->sql(std::string("CREATE TABLE ") + _dbTables[createIndex[i]][0] + "(" + _getConfParam(_dbTables[createIndex[i]]) + ")").execute();
	}
	return 0;

}

bool SQLDataBase::isDatabaseExist()
{
		mysqlx::Row row;
		mysqlx::SqlResult resp = _sqlSession->sql(std::string("SHOW DATABASES ")).execute();
		bool isdbFound = 0;
		while (row = resp.fetchOne())
		{
			if (std::string(row[0]) == _dbSchemaName)
				isdbFound = 1;
		}
	return isdbFound;
}

std::vector<int> SQLDataBase::isTableExist()
{
	mysqlx::Row row;
	mysqlx::SqlResult resp = _sqlSession->sql(std::string("SHOW TABLES ")).execute();
	std::vector<int> index;
	std::vector<std::string> getTable;
	while (row = resp.fetchOne())
	{
		getTable.push_back(std::string(row[0]));
	}
	
	
	for (int i = 0; i < _dbTables.size(); i++)
	{
		bool isIdentical = 0;
		for (int j = 0; j < getTable.size(); j++)
		{
			if (getTable[j].compare(_dbTables[i][0]) == 0)
			{
				isIdentical = 1;
			}
		}
		if (isIdentical == 0)
		{
			index.push_back(i);
		}
		
	}
	return index;
}

void SQLDataBase::createEntity(std::string atribute, std::string value, std::string TableName)
{
	_sqlSession->sql (std::string("INSERT INTO ") + TableName + "(" + atribute + ")" + ("VALUE ") + "(" + value + ")").execute();	
}

std::string SQLDataBase::updateEntity()
{
	//_sqlSession->sql(std::string("UPDATE ") + /*"Table name "*/ + "SET " + /*"Atribute name"*/ + "= " + /*"Value in table "*/ + "WHERE " /*"Atribute name"*/ + "= " + /*"new value*/).execute();
	return std::string();
}

std::string SQLDataBase::deleteEntity()
{
	//_sqlSession->sql(std::string("DELETE FROM ") + /*"NameTAble"*/ +"WHERE " + /*Atribute name*/ + "= " + /*value*/).execute();
	return std::string();
}

SQLDataBase::~SQLDataBase()
{
}
