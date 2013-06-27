#pragma once

#include <socket.io-serverpp/config.hpp>
#include <socket.io-serverpp/scgi/Service.h>
#include <socket.io-serverpp/Message.hpp>
#include <socket.io-serverpp/SocketNamespace.hpp>
#include <socket.io-serverpp/uuid.hpp>

#include <sstream>

#include <boost/regex.hpp>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

class SocketNamespace;

typedef scgi::Service<asio::local::stream_protocol> scgiserver;

class Server
{
    public:
    Server(asio::io_service& io_service)
    :m_io_service(io_service)
    ,m_reSockIoMsg("^(\\d):([\\d+]*):([^:]*):?(.*)", boost::regex::perl)
    {
        m_sockets = this->of("");

        m_protocols = {"websocket"};

        m_wsserver.init_asio(&io_service);
        m_wsserver.set_message_handler(bind(&Server::onWebsocketMessage, this, _1, _2));
        m_wsserver.set_open_handler(bind(&Server::onWebsocketOpen, this, _1));
        m_wsserver.set_close_handler(bind(&Server::onWebsocketClose, this, _1));
        
        m_scgiserver.sig_RequestReceived.connect(bind(&Server::onScgiRequest, this, _1));
    }

    void listen(const string& scgi_socket, int websocket_port)
    {
        auto acceptor = std::make_shared<scgiserver::proto::acceptor>(m_io_service, scgiserver::proto::endpoint(scgi_socket));
        m_scgiserver.listen(acceptor);
        m_scgiserver.start_accept();

        m_wsserver.listen(websocket_port);
        m_wsserver.start_accept();
    }

    shared_ptr<SocketNamespace> of(const string& nsp)
    {
        auto iter = m_socket_namespace.find(nsp);
        if (iter == m_socket_namespace.end())
        {
            auto snsp = make_shared<SocketNamespace>(nsp, m_wsserver);
            m_socket_namespace.insert(std::make_pair(nsp, snsp));
            return snsp;
        }
        else
        {
            return iter->second;
        }
    }

    shared_ptr<SocketNamespace> sockets()
    {
        return m_sockets;
    }

    private:
    void onScgiRequest(scgiserver::CRequestPtr req)
    {
        string uri = req->header("REQUEST_URI");
        string uuid = uuid::uuid1();
        std::cout << "Req: " << req->header("REQUEST_METHOD") << "  Uri: " << uri << std::endl;

        if (uri.find("/socket.io/1/") == 0)
        {
            std::ostringstream os;
            os << "Status: 200 OK\r\n";
            os << "Content-Type: text/plain\r\n\r\n";
            os << uuid + ":" << m_heartBeat << ":" << m_reconnectTime << ":";
            for (const auto& p : m_protocols)
            {
                os << p << ",";
            }

            req->writeData(os.str());
            req->asyncClose(std::bind([](scgiserver::CRequestPtr req){ cout << "closed" << endl;}, req));
        }
    }

    
    void onWebsocketOpen(wspp::connection_hdl hdl)
    {
        auto connection = m_wsserver.get_con_from_hdl(hdl);
        string uuid = connection->get_resource().substr(23);

        //m_websocketServer.send(hdl, "5::{name:'connect', args={}}", ws::frame::opcode::value::text);
        m_wsserver.send(hdl, "1::", wspp::frame::opcode::value::text);
        //    m_websocketServer.set_timer(10*1000, bind(&SocketIoServer::sendHeartbeart, this, hdl));
        //    m_websocketServer.set_timer(1*1000, bind(&SocketIoServer::customEvent, this, hdl));
    }

    /*
     * [message type] ':' [message id ('+')] ':' [message endpoint] (':' [message data])
     *
     * 0::/test disconnect a socket to /test endpoint
     * 0 disconnect whole socket
     * 1::/test?my=param
     * 3:<id>:<ep>:<data>
     * 4:<id>:<ep>:<json>
     * 5:<id>:<ep>:<json event>
     * 6:::<id>
     *
     * axe:
     * msgtype := numeric
     * fs := :
     * msgid := numeric
     * msgiduser := +
     * endpoint := alphanum
     * data := all
     * socketiomsg := msgtype + fs + msgid* + msgiduser* + fs + endpoint* + fs* + data*
     * regex: "\d:\d*\+*:[^:]*:.*"
     */

    void onWebsocketMessage(wspp::connection_hdl hdl, wsserver::message_ptr msg)
    {
        string payload = msg->get_payload();
        if (payload.size() < 3)
            return;

        boost::smatch match;
        if (boost::regex_match(payload, match, m_reSockIoMsg))
        {
            Message message = {
                false,
                "",
                payload[0] - '0',
                0,
                false,
                match[3],
                match[4]
            };

            switch(payload[0])
            {
                case '0': // Disconnect
                    break;
                case '1': // Connect
                    {
                        // signal connect to matching namespace
                        auto socket_namespace = m_socket_namespace.find(message.endpoint);
                        if (socket_namespace != m_socket_namespace.end())
                        {
                            socket_namespace->second->onSocketIoConnection(hdl);
                        }

                        m_wsserver.send(hdl, payload, wspp::frame::opcode::value::text);
                    }
                    break;
                case '4': // JsonMessage
                        message.isJson = true;
                case '3': // Message
                        {
                            auto socket_namespace = m_socket_namespace.find(message.endpoint);
                            if (socket_namespace != m_socket_namespace.end())
                            {
                                socket_namespace->second->onSocketIoMessage(hdl, message);
                            }

                            std::cout << "Message: " << payload << std::endl;
                        }
                case '5': // Event
                        
                    break;
            }
        }

        std::cout << msg->get_payload() << std::endl;
    }

    void onWebsocketClose(wspp::connection_hdl hdl)
    {
    }

    asio::io_service& m_io_service;
    wsserver m_wsserver;
    scgiserver m_scgiserver;
    shared_ptr<SocketNamespace> m_sockets;
    map<string, shared_ptr<SocketNamespace>> m_socket_namespace;
    boost::regex m_reSockIoMsg;
    int m_heartBeat;
    int m_reconnectTime;
    vector<string> m_protocols;
};

}

    using lib::Server;
}
