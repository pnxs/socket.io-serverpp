#pragma once

#include <socket.io-serverpp/config.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

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

using lib::Message;

}
