#include <socketio-serverpp/server.hpp>

/* This example only a draft of the interface that
 * should be implemented.
 */

int main()
{
    socketio-serverpp::server server;;

    server.listen(9000);

    server.sockets.on(socketio-serverpp::event::connection, [](socketio-serverpp::socket socket)
    {
        socket.emit('my event', 'some data');
        socket.on('other event', [](const string& data)
        {
            cout << data << endl;
        });
    });

    server.run();
}
