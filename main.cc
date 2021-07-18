#include <drogon/drogon.h>
using namespace drogon;
using namespace std;
using namespace drogon::orm;
int main() {
    //Set HTTP listener address and port
    app().addListener("0.0.0.0",8000);
    //Load config file
    app().loadConfigFile("../config.json");
    //Run HTTP framework,the method will block in the internal event loop
    app().registerHandler("/",
                          [](const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) {
                              auto resp = HttpResponse::newHttpResponse();
                              //THIS CONNECTION WORKING FINE :-
                              //auto db = DbClient::newPgClient("postgres://milthncaegivda:81a187b80654ed43bb093cecaeba9397999dc754b8dbfa9cc5b861f82f48afb7@ec2-52-202-152-4.compute-1.amazonaws.com:5432/d80qgv501kv1q9",2);
                              //note that we shouldn't use a sync interface of a FastDbClient;
                              auto db = app().getDbClient();
                              auto res = db->execSqlSync("select * from test");
                              if (res.affectedRows())
                                  resp->setBody(res[0]["id"].as<string>());
                              else
                                  resp->setBody("Nothing ");

                              callback(resp);
                          }, {Get, ""});
    app().run();
    return 0;
}
