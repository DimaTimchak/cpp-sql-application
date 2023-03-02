
extern "C" {}
#include <iostream>
#include <string>
#include <mysqlx/xdevapi.h>
#include <boost/asio.hpp>
#include "crow_all.h"
#include "SQLDataBase.h" //don use this call ins


using ::std::cout;
using ::std::endl;
using namespace ::mysqlx;

int databaseExample ()try {

  

    const char* url = ("mysqlx://root@127.0.0.1");

    cout << "Creating session on " << url
        << " ..." << endl;

    Session sess(SessionOption::HOST, "localhost",
        SessionOption::PORT, 33060,
        SessionOption::USER, "root",
        SessionOption::PWD, "");
    ;

    {
        /*
          TODO: Only working with server version 8
        */

        RowResult res = sess.sql("show variables like 'version'").execute();
        std::stringstream version;
        
        version << res.fetchOne().get(1).get<string>();
        int major_version;
        version >> major_version;

        if (major_version < 8)
        {
            cout << "Done!" << endl;
            return 0;
        }
    }


    cout << "Session accepted..." << endl;
    
    Schema sch = sess.createSchema("test", true);
    Collection coll = sch.createCollection("c1", true);

    cout << "Inserting documents..." << endl;

    coll.remove("true").execute();
    
    {
        Result add;

        add = coll.add(R"({ "name": "foo", "age": 1 })").execute();
        std::vector<string> ids = add.getGeneratedIds();
        cout << "- added doc with id: " << ids[0] << endl;

        add = coll.add(R"({ "name": "bar", "age": 2, "toys": [ "car", "ball" ] })")
            .execute();
        if (ids.size() != 0)
            cout << "- added doc with id: " << ids[0] << endl;
        else
            cout << "- added doc" << endl;

        add = coll.add(R"({
       "name": "baz",
        "age": 3,
       "date": { "day": 20, "month": "Apr" }
    })").execute();
        if (ids.size() != 0)
            cout << "- added doc with id: " << ids[0] << endl;
        else
            cout << "- added doc" << endl;

        add = coll.add(R"({ "_id": "myuuid-1", "name": "foo", "age": 7 })")
            .execute();
        ids = add.getGeneratedIds();
        if (ids.size() != 0)
            cout << "- added doc with id: " << ids[0] << endl;
        else
            cout << "- added doc" << endl;
    }

    cout << "Fetching documents..." << endl;

    DocResult docs = coll.find("age > 1 and name like 'ba%'").execute();

    DbDoc doc = docs.fetchOne();

    for (int i = 0; doc; ++i, doc = docs.fetchOne())
    {
        cout << "doc#" << i << ": " << doc << endl;

        for (Field fld : doc)
        {
            cout << " field `" << fld << "`: " << doc[fld] << endl;
        }

        string name = doc["name"];
        cout << " name: " << name << endl;
        
        if (doc.hasField("date") && Value::DOCUMENT == doc.fieldType("date"))
        {
            cout << "- date field" << endl;
            DbDoc date = doc["date"];
            for (Field fld : date)
            {
                cout << "  date `" << fld << "`: " << date[fld] << endl;
            }
            string month = doc["date"]["month"];
            int day = date["day"];
            cout << "  month: " << month << endl;
            cout << "  day: " << day << endl;
        }

        if (doc.hasField("toys") && Value::ARRAY == doc.fieldType("toys"))
        {
            cout << "- toys:" << endl;
            for (auto toy : doc["toys"])
            {
                cout << "  " << toy << endl;
            }
        }

        cout << endl;
    }

    cout << "Done!" << endl;
}
catch (const mysqlx::Error& err)
{
    cout << "ERROR: " << err << endl;
    return 1;
}
catch (std::exception& ex)
{
    cout << "STD EXCEPTION: " << ex.what() << endl;
    return 1;
}
catch (const char* ex)
{
    cout << "EXCEPTION: " << ex << endl;
    return 1;
}
std::string vectorArr[50];
int bynSech(int *arr,int size,int target_value) {
    
    int low = 0;
    int high = size - 1;
    
    while (low < high) {
        int mid = (high + low) / 2;
        if (target_value == arr[mid]) {
            return mid;
        }
        if (target_value > arr[mid]) {
            low = mid + 1;
        }
        if (target_value < arr[mid]) {
            low = mid - 1;
        }
    }
    return 0;
}

int main()
{
    try {
        SQLDataBase obje;
        obje.initDataBase();
    }
    catch (std::exception& e){
        std::cerr<<"\nError occurd: " << e.what()<<"\n";
        return 1;
    }




    std::vector<std::string> responseArr;
   // databaseExample();
    std::cout << "\n\n--- ---\n";
	crow::SimpleApp app; //define your crow application
    CROW_ROUTE(app, "/")([]() {
        // crow::mustache::context view_context;
        auto home_page = crow::mustache::load(R"(index.html)");
    return home_page.render();
        });
   
 
    CROW_ROUTE(app, "/post").methods(crow::HTTPMethod::Post)
        ([](const crow::request& req) {
        crow::json::wvalue x = crow::json::load(req.body);
        if (!x.size())return crow::response(400);
        std::string resp= std::to_string(x.keys().size());
        for (int i = 0; i < stoi(resp); i++) {
            std::string response = x[x.keys()[i]].dump();
            std::cout << i << ": " << response << endl;
            vectorArr[i] = response;
            
        }
        return crow::response{("\nanswer: ")+resp};
            });

    CROW_ROUTE(app, "/get").methods(crow::HTTPMethod::Get)
        ([]() {
        
        std::ostringstream os;
    for (int i = 0; i < 50; i++) {
        if(vectorArr[i]!=""){
        os << vectorArr[i]<<" ";
        }
    }
    return crow::response{ os.str()};
            });

  

    CROW_ROUTE(app, "/params")
        ([](const crow::request& req) {
        std::ostringstream os;

   
    // To get a simple string from the url params
    // To see it in action /params?foo='blabla'
    os << "Params: " << req.url_params << "\n\n";
    os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";

    // To get a double from the request
    // To see in action submit something like '/params?pew=42'
    if (req.url_params.get("pew") != nullptr) {
        double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
        os << "The value of 'pew' is " << countD << '\n';
    }

    // To get a list from the request
    // You have to submit something like '/params?count[]=a&count[]=b' to have a list with two values (a and b)
    auto count = req.url_params.get_list("count");
    os << "The key 'count' contains " << count.size() << " value(s).\n";
    for (const auto& countVal : count) {
        os << " - " << countVal << '\n';
    }

    // To get a dictionary from the request
    // You have to submit something like '/params?mydict[a]=b&mydict[abcd]=42' to have a list of pairs ((a, b) and (abcd, 42))
    auto mydict = req.url_params.get_dict("mydict");
    os << "The key 'dict' contains " << mydict.size() << " value(s).\n";
    for (const auto& mydictVal : mydict) {
        os << " - " << mydictVal.first << " -> " << mydictVal.second << '\n';
    }

    return crow::response{ os.str() };
            });
	
	app.port(18080).multithreaded().run();
	cout << "ended";
	return 0;
}
