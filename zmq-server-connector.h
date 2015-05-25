#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <map>
#include <jsonrpccpp/server/abstractserverconnector.h>
#include "zmq.hpp"

using namespace jsonrpc;

/**
* This class provides an embedded HTTP Server, based on libmicrohttpd, to handle incoming Requests and send HTTP 1.1
* valid responses.
* Note that this class will always send HTTP-Status 200, even though an JSON-RPC Error might have occurred. Please
* always check for the JSON-RPC Error Header.
*/
class ZmqServerConnector: public AbstractServerConnector
{
public:

    /**
     * @brief HttpServer, constructor for the included HttpServer
     * @param port on which the server is listening
     * @param enableSpecification - defines if the specification is returned in case of a GET request
     * @param sslcert - defines the path to a SSL certificate, if this path is != "", then SSL/HTTPS is used with the given certificate.
     */
    ZmqServerConnector(int port, const std::string& sslcert = "", const std::string& sslkey = "", int threads = 50);

    virtual bool StartListening();
    virtual bool StopListening();
    virtual bool SendResponse(const std::string& response, void* addInfo = NULL);

private:
    int port;
    int threads;
    bool running;
    std::string path_sslcert;
    std::string path_sslkey;
    std::string sslcert;
    std::string sslkey;

    zmq::context_t context;
    zmq::socket_t socket;
};

