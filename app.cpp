// my first program in C++
#include <iostream>
#include <string>
#include <unistd.h>


#include "zmq-server-connector.h"

#include <jsonrpccpp/server.h>


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
    return 0;
}
