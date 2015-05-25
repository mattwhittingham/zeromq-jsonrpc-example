// my first program in C++
#include <iostream>
#include <string>
#include <unistd.h>


#include "mysql_connection.h"
#include "zmq-server-connector.h"

#include <jsonrpccpp/server.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace jsonrpc;
using namespace std;


class SampleServer : public AbstractServer<SampleServer>
{
    public:
        SampleServer(ZmqServerConnector &server) :
            AbstractServer<SampleServer>(server)
        {
            this->bindAndAddMethod(Procedure("sayHello", PARAMS_BY_NAME, JSON_STRING, "name", JSON_STRING, NULL), &SampleServer::sayHello);
            this->bindAndAddNotification(Procedure("notifyServer", PARAMS_BY_NAME, NULL), &SampleServer::notifyServer);
        }

        //method
        void sayHello(const Json::Value& request, Json::Value& response)
        {
            response = "Hello: " + std::to_string(::getpid()) + " : " + request["name"].asString();
        }

        //notification
        void notifyServer(const Json::Value& request)
        {
            (void)request;
            cout << "server received some Notification" << endl;
        }
};

int main(int argc, char *argv[])
{
    int opt, port;

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-p port]\n",
                    argv[0]);
            return EXIT_FAILURE;
        }
    }

    try
    {
        ZmqServerConnector server(port);
        SampleServer serv(server);
        if (serv.StartListening())
        {
            cout << "Server started successfully" << endl;
            getchar();
            serv.StopListening();
        }
        else
        {
            cout << "Error starting Server" << endl;
        }
    }
    catch (jsonrpc::JsonRpcException& e)
    {
        cerr << e.what() << endl;
    }
    // zmq::context_t context (1);
    // zmq::socket_t socket (context, ZMQ_REP);
    // socket.bind ("tcp://*:5555");

    // while (true) {
    //     zmq::message_t request;

    //     //  Wait for next request from client
    //     socket.recv (&request);
    //     std::cout << "Received Hello" << std::endl;

    //     //  Do some 'work'
    //     // sleep(1);

    //     //  Send reply back to client
    //     zmq::message_t reply (5);
    //     memcpy ((void *) reply.data (), "World", 5);
    //     socket.send (reply);
    // }
    return 0;
    // sql::Driver *driver;
    // sql::Connection *con;
    // sql::Statement *stmt;
    // sql::ResultSet  *res;

    // driver = get_driver_instance();
    // con = driver->connect("tcp://127.0.0.1:3306", "root", "");
    // con->setSchema("spinminute");

    // stmt = con->createStatement();
    // res = stmt->executeQuery("SELECT 'Hello World!' AS _message");

    // while (res->next()) {
	   //  std::cout << "\t... MySQL replies: ";
	   //  /* Access column data by alias or column name */
	   //  std::cout << res->getString("_message") << std::endl;
	   //  std::cout << "\t... MySQL says it again: ";
	   //  /* Access column fata by numeric offset, 1 is the first column */
	   //  std::cout << res->getString(1) << std::endl;
    // }

    // delete res;
    // delete stmt;
    // delete con;
}
