#pragma once

#define SOCKETIO_SERVERPP_NAMESPACE socketio_serverpp

#include <socket.io-serverpp/common/cppconfig.hpp>

#define _WEBSOCKETPP_CPP11_STL_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <boost/signals2.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

typedef string SessionId;
typedef string Room;
namespace asio = boost::asio;
namespace wspp = websocketpp;

using boost::signals2::signal;

typedef wspp::server<wspp::config::asio> wsserver;

}
}
