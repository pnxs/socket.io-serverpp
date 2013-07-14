#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/Message.hpp>
#include <socket.io-serverpp/Event.hpp>

#include <socket.io-serverpp/lib/rapidjson/document.h>

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

    void on(const std::string& event, function<void (const Event& data)> cb)
    {
//        cout << "register event '" << event << "'" << this << endl;
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
//        cout << "Socket send: " << data << endl;
    }

    void emit(const string& name, const string& data)
    {
        string pl = "5::" + m_namespace + ":{\"name\":\"" + name + "\",\"args\":[\""+data+"\"]}";
        m_wsserver.send(m_ws_hdl, pl, wspp::frame::opcode::value::text);
//        cout << "Socket emit: " << name << " data: " << data << endl;
    }

    void onMessage(const Message& msg)
    {
        auto iter = m_events.find("message");
        if (iter != m_events.end())
        {
            iter->second({"message", msg.data});
        }
    }

    void onEvent(const string& event, const rapidjson::Document& json, const string& rawJson)
    {
//        cout << "Socket check '" << event << "'" << this << endl;
        auto iter = m_events.find(event);
        if (iter != m_events.end())
        {
            iter->second({event, json, rawJson});
//            cout << "Socket event: " << event << " matched" << endl;
        }
    }

    wsserver&               m_wsserver;
    const string&           m_namespace;
    wspp::connection_hdl    m_ws_hdl;
    map<string, function<void (const Event&)>> m_events;
};

}
    using lib::Socket;
}
