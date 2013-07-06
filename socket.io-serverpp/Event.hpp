#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/Message.hpp>

#include <socket.io-serverpp/lib/rapidjson/document.h>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

class Event
{
    public:
    Event(const string& event, const rapidjson::Document& json)
    :m_isJson(true), m_event(event), m_json(&json)
    {
    }
    
    Event(const string& event, const string& data)
    :m_isJson(false), m_event(event), m_json(0), m_stringdata(data)
    {
    }

    bool isJson() const
    {
        return m_isJson;
    }

    string name() const
    {
        return m_event;
    }

    string data() const
    {
        return m_stringdata;
    }

    bool   m_isJson;
    string m_event;
    const rapidjson::Document* m_json;
    string m_stringdata;
};

}

using lib::Event;

}
