#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/Server.hpp>
#include <socket.io-serverpp/Socket.hpp>

#include <boost/signals2.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{

    using std::cout;
    using std::endl;

class Server;
class Socket;

class SocketNamespace
{
    friend Server;
    public:
    SocketNamespace(const string& nsp, wsserver& server)
    :m_namespace(nsp)
    ,m_wsserver(server)
    {
    }

    void onConnection(function<void (Socket&)> cb)
    {
        sig_Connection.connect(cb);
    }

    void on(const string& event, function<void (const string&)> cb)
    {
        // add cb to event-signal
        //m_events[event]; //.connect(cb);
    }

    //list<Client> clients(const string& room) const;
    //void except(const SessionId& id);
    void send(const string& data)
    {
        cout << "SocketNamespace send: " << data << endl;
        for (const auto& i : m_sockets)
        {
            i.second->send(data);
        }
    }

    void emit(const string& name, const string& data)
    {
        cout << "SocketNamespace emit: " << name << " data: " << data << endl;
        for (const auto& i : m_sockets)
        {
            i.second->emit(name, data);
        }
    }

    string socketNamespace() const
    {
        return m_namespace;
    }
    
    private:

    void onSocketIoConnection(websocketpp::connection_hdl hdl)
    {
        auto socket = make_shared<Socket>(m_wsserver, m_namespace, hdl);
        //m_sockets[hdl] = socket;
        sig_Connection(*socket);
    }

    void onSocketIoMessage(websocketpp::connection_hdl hdl, const Message& msg)
    {
        cout << "SocketNamespace(" << m_namespace << ") msg: " << msg.data << endl;
        auto iter = m_sockets.find(hdl);
        if (iter != m_sockets.end())
        {
            iter->second->onMessage(msg);
        }
    }

    string m_namespace;
    wsserver& m_wsserver;

    boost::signals2::signal<void (Socket&)> sig_Connection;
    map<websocketpp::connection_hdl, shared_ptr<Socket>, std::owner_less<websocketpp::connection_hdl>> m_sockets;
};

}

