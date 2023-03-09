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
���� 1: ������� ���� �� ��������� � ���������� �������������� ������
1 - ����� http �������� ���� ����������� (username,email,password,additional) ���� �������� �������� ����������� ������ �� ������ �������� ������ (����. ����� unsernames)
2 - ������������� ����� ������ ����������� ����� web �������
--
���� 2 (����� ���������� ���� ����� ��������� 1 ����) : ��������� ������������, ������� ���� �������� �������������� ������ �� 2 �����
1 - �������� ���� �� ������� ���������� ����� e-mail, � ��� ���� ���������� �� ���������� ��� �������� 30�� ������� ����������
2 - ��� ����������� e-mail ������� ���� � ������� ���� �����
3 - � ��������� �� ���� �� �������������� ���������� �������� ��� ���������� �� ���� ������
*/

