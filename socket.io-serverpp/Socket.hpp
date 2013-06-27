#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/Message.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

class SocketNamespace;

class Socket
{
    friend SocketNamespace;
    public:
    Socket(wsserver& wsserver, const string& nsp, wspp::connection_hdl hdl)
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
        m_wsserver.send(m_ws_hdl, pl, wspp::frame::opcode::value::text);
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

    wsserver&               m_wsserver;
    const string&           m_namespace;
    wspp::connection_hdl    m_ws_hdl;
    map<string, function<void (const string& data)>> m_events;
};

}
    using lib::Socket;
}
