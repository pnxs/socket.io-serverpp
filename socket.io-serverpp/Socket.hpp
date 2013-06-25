#pragma once

#define _WEBSOCKETPP_CPP11_STL_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/Message.hpp>
#include <boost/signals2.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{

    using std::cout;
    using std::endl;

class SocketNamespace;

typedef websocketpp::server<websocketpp::config::asio> wsserver;

class Socket
{
    friend SocketNamespace;
    public:
    Socket(wsserver& wsserver, const string& nsp, websocketpp::connection_hdl hdl)
    :m_wsserver(wsserver)
    ,m_namespace(nsp)
    ,m_ws_hdl(hdl)
    {
    }

    void on(const std::string& event, function<void (const string& data)> cb)
    {
        m_events[event] = cb;
    }

/*
    void on(const std::string& event, function<void (const string& from, const string& data)> cb)
    {
    }
    */
    
    void send(const string& data)
    {
        string pl = "3::" + m_namespace + ":" + data;
        m_wsserver.send(m_ws_hdl, pl, websocketpp::frame::opcode::value::text);
        cout << "Socket send: " << data << endl;
    }

    void emit(const string& name, const string& data)
    {
        cout << "Socket emit: " << name << " data: " << data << endl;
    }

    void onMessage(const Message& msg)
    {
        auto iter = m_events.find("message");
        if (iter != m_events.end())
        {
            iter->second(msg.data);
        }
    }

    wsserver& m_wsserver;
    const string&   m_namespace;
    websocketpp::connection_hdl m_ws_hdl;
    map<string, function<void (const string& data)>> m_events;
};

}
