#pragma once
#include <string>
#include <memory>
class BackendDataBase {
	protected:
	
	std::string _configureFilePath;

	std::string _dbName;
	std::string _dbHost;
	int _dbPort;
	std::string _dbUser;
	std::string _dbPassword;
	std::string _dbSchemaName;
	public:
		BackendDataBase(std::string configureFilePath) :_configureFilePath(configureFilePath) {};
	virtual int initDataBase() = 0; //set up BackendDataBase implementation and create and init dataDatabase if it is not created;
	virtual bool isDatabaseExist() = 0; // cheks is database configured and ready to use (for MySQL it cheks is coresponding scheme/structure exist )
	virtual void setPath(std::string configureFilePath)=0;
	//std::string getDataBaseName() { return _dbName; } 
	virtual void createEntity(std::string atribute, std::string value, std::string TableName) = 0; 
	virtual std::string readEntity(std::string TableName, int page) = 0;
	virtual void updateEntity(std::string tableName,  std::string value, std::string id) = 0;
	virtual void deleteEntity(std::string table, std::string entityId) = 0;
	virtual ~BackendDataBase() {};
};
/*
Етап 1: основна ідея це отримання і збереження користувацьких данних
1 - через http приймати данні користувача (username,email,password,additional) після валідації надавати користувачу доступ до іньших відкритих данних (напр. іньші unsernames)
2 - альтернативно можна видати користувачу якусь web сторінку
--
Етап 2 (можна реалізувати після повної реалізації 1 еапу) : реєстрація користувачів, розбити етап загрузки користувацьких данних на 2 етапи
1 - тимчасові данні які потрібно підтвердити через e-mail, в разі якщо користувач не підтрведить дані протягом 30хв очитити інформацію
2 - при підтвердженні e-mail занести данні в постійну базу даних
3 - в залежності від того чи зарегістрований користувач видавати різні результати на певні запити
*/

