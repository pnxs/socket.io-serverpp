#pragma once

#include <socket.io-serverpp/config.hpp>

#include <boost/signals2.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{

class SocketNamespace
{
    public:
    SocketNamespace(const string& nsp)
    :m_namespace(nsp)
    {
    }

    void on(const string& event, function<void (const string&)>& cb)
    {
        // add cb to event-signal
        //m_events[event]; //.connect(cb);
    }

    //list<Client> clients(const string& room) const;
    //void except(const SessionId& id);
    //void send(const string& data);
    //void emit(const string& name);

    private:
    string m_namespace;
    map<string, boost::signals2::signal<void (const string&)>> m_events;
};

}
