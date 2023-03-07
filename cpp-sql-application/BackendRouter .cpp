#include "BackendRouter.h"




BackendRouter::BackendRouter(BackendCore* corePtr, int serverPort, std::vector<std::string> avalibleOrigins)
    : _corePtr(corePtr), _serverPort(serverPort), _avalibleOrigins(avalibleOrigins)
{
    //_baseResponse.headers.clear();
}


void BackendRouter::initAllRoutes()
{
    
    //Route that can return index.html to your web browser
    CROW_CATCHALL_ROUTE(app, "/")([]() {
        // crow::mustache::context view_context;
        auto home_page = crow::mustache::load(R"(index.html)");
    return home_page.render();
        });


    //Route that return all Tables from config file 
    CROW_ROUTE(app, "/get/config").methods(crow::HTTPMethod::Get)
        ([&](const crow::request& req) {
        std::ifstream file("conf/user-database-conf.json");
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
    std::string responseData = (crow::json::wvalue(crow::json::load(confInternals))["dbTables"].dump());
    crow::response res;
    int corespondingIndex = CBDutils::findCorespondingIndexInAppFromValueInMap(_avalibleOrigins, req.headers, "Origin");
    if (corespondingIndex != _avalibleOrigins.size()) {
        res.add_header("Access-Control-Allow-Origin", _avalibleOrigins[corespondingIndex]);
    }
    res.body = (std::move(responseData));
    return res;
            });


    CROW_ROUTE(app, "/get/tableInfo").methods(crow::HTTPMethod::Get)
        ([&](const crow::request& req) {
        crow::response res;

    //returns index of element taht equal to req header origin
   int corespondingIndex= CBDutils::findCorespondingIndexInAppFromValueInMap(_avalibleOrigins, req.headers, "Origin");
    if(corespondingIndex != _avalibleOrigins.size()){
   res.add_header("Access-Control-Allow-Origin",_avalibleOrigins[corespondingIndex]);
    }

    std::string tableValue = req.url_params.get("table");
    if (tableValue == "") {
        res.body = 404;
    }
    else {

        std::string responseBody = _corePtr->handleReadTableAttr(tableValue);
        
        res.body = tableValue;
        
    }
    return res;

            });
    //Gets an object to add to the database
    CROW_ROUTE(app, "/addEntity").methods(crow::HTTPMethod::Post)
        ([&](const crow::request& req) {
        crow::json::wvalue x = crow::json::load(req.body);
    
    crow::response res;
    int corespondingIndex = CBDutils::findCorespondingIndexInAppFromValueInMap(_avalibleOrigins, req.headers, "Origin");
    if (corespondingIndex != _avalibleOrigins.size()) {
        res.add_header("Access-Control-Allow-Origin", _avalibleOrigins[corespondingIndex]);
    }
    res.body = (200);
    if (!x.size())res.body = (400);
    std::string resp = (x.dump());
    _corePtr->handleAddEntity(resp);
    
    return res;
            });
}

void BackendRouter::setOriginsArr(std::vector<std::string> avalibleOrigins)
{
    _avalibleOrigins = avalibleOrigins;
}

void BackendRouter::setBaseResponseHeaders(std::vector<std::string> originsArr)
{
    for (int i = 0; i < originsArr.size(); i++) {
        _baseResponse.add_header("Access-Control-Allow-Origin", originsArr[i]);
    }
}

void BackendRouter::runServer()
{
    initAllRoutes();
    app.port(_serverPort).multithreaded().run();
}
