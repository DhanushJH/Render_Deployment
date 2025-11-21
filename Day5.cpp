#include<iostream>
#include<fstream>
#include<iomanip>
#include<ctime>
#include<cstdlib>
#include<string>
#include<nlohmann/json.hpp>
#include "httplib.h"

using json = nlohmann::json;

json loadtojson()
{
    json DatainFormat = json::array();
    std::ifstream insidefile("Jokes_log.json");
    if(insidefile.good())
    {
        try
        {
            insidefile >> DatainFormat;
        }
        catch(const std::exception& e)
        {
            std::cout << "File is empty" << "\n";
        }
    }
    insidefile.close();

    return DatainFormat;
}

void savetofile(const json& DatainFormat)       // here constant is used as object/json will not be modified and it is read only, if it was to be modified like in deletejoke, remove const
{
    std::ofstream outputfile("Jokes_log.json");
    outputfile << std::setw(4) << DatainFormat << std::endl; 
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    httplib::Server svr;
    // Allow CORS Preflight
    svr.Options(".*", [](const httplib::Request &req, httplib::Response &res)
                {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS"); });

    svr.set_default_headers({{"Access-Control-Allow-Origin", "*"},
                             {"Access-Control-Allow-Headers", "*"},
                             {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"}});

    //GetAll

    svr.Get("/all", [](const httplib::Request& req, httplib::Response& res)
    {
        json db = loadtojson();
        res.set_content(db.dump(4), "application/json");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    //GetCount

    svr.Get("/count", [](const httplib::Request& req, httplib::Response& res)
    {
        json db = loadtojson();
        if(db.empty())
        {
            json err;
            err["error"] = "Database is empty";
            res.status = 404;
            res.set_content(err.dump(4), "application/json");
            return;
        }

        json out;
        out["count"] = db.size();
        res.set_content(out.dump(4), "application/json");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    //Get Random Joke
    svr.Get("/random", [](const httplib::Request& req, httplib::Response& res)
    {
        json db = loadtojson();
        if(db.empty())
        {
            json err;
            err["error"] = "Database is empty";
            res.status = 404;
            res.set_content(err.dump(4), "application/json");
            res.set_header("Access-Control-Allow-Origin", "*");
            return;
        }

        json out;
        int index = std::rand() % static_cast<int>(db.size());
        out = db[index];
        res.set_content(out.dump(4), "application/json");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    std::cout << "Server starting on http:://localhost:8080\n";
    std::cout << " /all    ->  Get all jokes\n";
    std::cout << " /count  ->  Get number of jokes in database\n";
    std::cout << " /random ->  Get a random joke\n";

    svr.listen("0.0.0.0", 8080);
    // svr.set_default_headers({"Access-Control-Allow-Origin", "*"});

    return 0;
}