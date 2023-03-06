
extern "C" {}
#include <iostream>
#include <string>
#include <mysqlx/xdevapi.h>
#include <boost/asio.hpp>
#include "crow_all.h"

#include "SQLDataBase.h"
#include "BackendCore.h"
#include "BackendRouter.h"
#include "CBDutils.h"
using ::std::cout;
using ::std::endl;
using namespace ::mysqlx;
int main()
{
    //std::string temp =(R"({"Name":"Ivan","Age":"15","Surname":"Zagora"})");
    //
    //crow::json::wvalue parsedTemp= (crow::json::load(temp));
    //for (int i = 0; i < parsedTemp.keys().size(); i++) {
    //    
    //    std::cout <<" " << CBDutils::cutQuotes( parsedTemp[ parsedTemp.keys()[i]].dump());
    //}

    std::cout << "\n\n--- ---\n";
    BackendCore backendApp(std::make_unique<SQLDataBase>(), "conf/user-database-conf.json",3000);
    backendApp.launchBackends();
    cout << "\nEnded\n";
	return 0;
}
