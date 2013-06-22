#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/SocketNamespace.hpp>

#define _WEBSOCKETPP_CPP11_STL_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <socket.io-serverpp/scgi/Service.h>

#include <uuid/uuid.h>

#include <iostream>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
typedef websocketpp::server<websocketpp::config::asio> wsserver;
typedef scgi::Service<boost::asio::local::stream_protocol> scgiserver;

using std::cout;
using std::endl;

namespace uuid
{
    string uuid1()
    {
        uuid_t uuid;
        char strId[38];
        uuid_generate_time(uuid);
        uuid_unparse(uuid, strId);
        strId[37] = 0;
        return strId;
    }
}


class Server
{
    public:
    Server(boost::asio::io_service& io_service)
    :m_io_service(io_service)
    ,m_sockets("")
    {
        m_wsserver.init_asio(&io_service);
        m_wsserver.set_message_handler(bind(&Server::onWebsocketMessage, this, _1, _2));
        m_wsserver.set_open_handler(bind(&Server::onWebsocketOpen, this, _1));
        m_wsserver.set_close_handler(bind(&Server::onWebsocketClose, this, _1));
        
        m_scgiserver.sig_RequestReceived.connect(bind(&Server::onScgiRequest, this, _1));
    }

    void listen(const string& scgi_socket, int websocket_port)
    {
        auto acceptor = std::make_shared<boost::asio::local::stream_protocol::acceptor>(m_io_service, boost::asio::local::stream_protocol::endpoint(scgi_socket));
        m_scgiserver.listen(acceptor);
        m_scgiserver.start_accept();

        m_wsserver.listen(websocket_port);
        m_wsserver.start_accept();
    }

    SocketNamespace of(const string& nsp)
    {
        return SocketNamespace(nsp);
    }

    SocketNamespace& sockets()
    {
        return m_sockets;
    }

    //boost::signals2<void(shared_ptr<SocketIoConnection>)> sig_onConnection;

    private:
    void onScgiRequest(scgiserver::CRequestPtr req)
    {
        string uri = req->header("REQUEST_URI");
        string uuid = uuid::uuid1();
        std::cout << "Req: " << req->header("REQUEST_METHOD") << "  Uri: " << uri << std::endl;

        if (uri.find("/socket.io/1/") == 0)
        {
            string data = "Status: 200 OK\r\nContent-Type: text/plain\r\n\r\n" + uuid + ":20:30:websocket";
            req->writeData(data);
            req->asyncClose(std::bind([](scgiserver::CRequestPtr req){ cout << "closed" << endl;}, req));
        }
    }

    
    void onWebsocketOpen(websocketpp::connection_hdl hdl)
    {
        auto connection = m_wsserver.get_con_from_hdl(hdl);
        string uuid = connection->get_resource().substr(23);

        //auto connection = make_shared<SocketIoConnection>(uuid);
        //m_connections[hdl] = connection;

        //m_websocketServer.send(hdl, "5::{name:'connect', args={}}", ws::frame::opcode::value::text);
        m_wsserver.send(hdl, "1::", websocketpp::frame::opcode::value::text);
        //    m_websocketServer.set_timer(10*1000, bind(&SocketIoServer::sendHeartbeart, this, hdl));
        //    m_websocketServer.set_timer(1*1000, bind(&SocketIoServer::customEvent, this, hdl));

        //sig_onConnection(connection);
    }

    void onWebsocketMessage(websocketpp::connection_hdl hdl, wsserver::message_ptr msg)
    {
        string payload = msg->get_payload();
        if (payload.size() < 3)
            return;

        switch(payload[0])
        {
            case '0': // Disconnect
                break;
            case '1': // Connect
                m_wsserver.send(hdl, payload, websocketpp::frame::opcode::value::text);
                break;
            case '3': // Message
                std::cout << "Message: " << payload << std::endl;

                break;
        }

        std::cout << msg->get_payload() << std::endl;
    }

    void onWebsocketClose(websocketpp::connection_hdl hdl)
    {
    }



    boost::asio::io_service& m_io_service;
    wsserver m_wsserver;
    scgiserver m_scgiserver;
    SocketNamespace m_sockets;
};

}

