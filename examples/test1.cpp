#include <iostream>

#define _SOCKETIO-SERVERPP_CPP11_STL_ 1

#include <socket.io-serverpp/Server.hpp>

using namespace std;

int main()
{
    boost::asio::io_service io_service;

    socketio_serverpp::Server io(io_service);
    
    unlink("/tmp/dorascgi");
    io.listen("/tmp/dorascgi", 9001);

    //io.sockets.on("connection", [](socketio-serverpp::socket socket)
#if 0
    io.sockets().on("connection", [](socketio-serverpp::socket socket)
    {
        socket.emit('my event', 'some data');
        socket.on('other event', [](const string& data)
        {
            cout << data << endl;
        });
    });
#endif

    auto chat = io.of("/chat");
    chat->onConnection([&](socketio_serverpp::Socket socket)
    {
        cout << "a socket with namespace /chat connected" << endl;

        socket.emit("a message", "only socket will get");
        chat->emit("a message", "all in /chat will get");
    });

    io_service.run();
}
