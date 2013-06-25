#pragma once

#define _WEBSOCKETPP_CPP11_STL_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <socket.io-serverpp/config.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{


typedef websocketpp::server<websocketpp::config::asio> wsserver;

// encoding text,json

struct Message
{
    bool isJson;
    string sender;

    int type;
    int messageId;
    bool idHandledByUser;
    string endpoint;
    string data;
};

}
