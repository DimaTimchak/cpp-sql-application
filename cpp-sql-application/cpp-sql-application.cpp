
extern "C" {}
#include <iostream>
#include <string>
#include <mysqlx/xdevapi.h>
#include <boost/asio.hpp>
#include "crow_all.h"

#include "SQLDataBase.h"
#include "BackendCore.h"
#include "BackendRouter.h"
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

    std::cout << "\n\n--- ---\n";
    BackendCore backendApp(std::make_unique<SQLDataBase>(), "conf/user-database-conf.json",3000);
    backendApp.launchBackends();
    cout << "\nEnded\n";
	return 0;
}
