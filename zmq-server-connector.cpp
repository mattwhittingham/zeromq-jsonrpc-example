/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    ZmqServerConnector.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <jsonrpccpp/common/specificationparser.h>

#include "zmq-server-connector.h"
#include "zmq-helpers.hpp"

using namespace jsonrpc;
using namespace std;

#define BUFFERSIZE 65536


ZmqServerConnector::ZmqServerConnector(int port, const std::string &sslcert, const std::string &sslkey, int threads) :
    AbstractServerConnector(),
    port(port),
    threads(threads),
    running(false),
    path_sslcert(sslcert),
    path_sslkey(sslkey),
    context(1),
    socket(this->context, ZMQ_REP)
{
}

bool ZmqServerConnector::StartListening()
{
    if(!this->running)
    {
        std::string uri = "tcp://localhost:" + std::to_string(this->port);
        this->socket.connect(uri.c_str());
        if (this->context != NULL)
            this->running = true;

        while (true) {

            //  Wait for next request from client
            std::string request = s_recv(this->socket);
            std::cout << "Received " << request << std::endl;

            //  Do some 'work'
            // sleep(1);
            std::string response;
            IClientConnectionHandler* handler = AbstractServerConnector::GetHandler();
            handler->HandleRequest(request, response);

            s_send(this->socket, response);
        }

    }
    return this->running;
}

bool ZmqServerConnector::StopListening()
{
    if(this->running)
    {
        this->socket.close();
        this->context.close();
        this->running = false;
    }
    return true;
}

bool ZmqServerConnector::SendResponse(const string& response, void* addInfo)
{
    //  send reply back to client
    return true;
}
