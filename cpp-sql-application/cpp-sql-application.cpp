
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
 
    std::cout << "\n\n--- ---\n";
    BackendCore backendApp(std::make_unique<SQLDataBase>(), "conf/user-database-conf.json",3000);
    backendApp.launchBackends();
    cout << "\nEnded\n";
	return 0;
}
