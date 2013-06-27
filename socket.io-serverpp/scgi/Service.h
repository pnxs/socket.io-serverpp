#pragma once

#include <boost/asio.hpp>
#include <socket.io-serverpp/scgi/Netstring.h>
#include <socket.io-serverpp/scgi/Request.h>
#include <memory>

namespace scgi
{
//namespace Internal
//{
namespace P = std::placeholders;
namespace A = boost::asio;
namespace S = boost::signals2;
using std::string;
using A::ip::tcp;
using boost::system::error_code;
using std::shared_ptr;

template <class PROTOCOL>
class Service
{
    public:
        using CRequest = Request<typename PROTOCOL::socket>;
        using CRequestPtr = shared_ptr<CRequest>;

        typedef PROTOCOL proto;

        void listen(shared_ptr<typename PROTOCOL::acceptor> acceptor)
        {
            m_acceptor = acceptor;
        }
        
        void start_accept()
        {
            auto request = std::make_shared<CRequest>(m_acceptor->get_io_service());

            m_acceptor->async_accept(request->socket(), bind(&Service::onAccept, this, request, P::_1));
        }

        S::signal<void (CRequestPtr)> sig_RequestReceived;

    private:

        void onAccept(CRequestPtr request, const error_code& error)
        {
            start_accept();
            if (!error) {
                request->receive(bind([&](CRequestPtr r) { sig_RequestReceived(r);}, request));
            }
        }

        shared_ptr<typename PROTOCOL::acceptor> m_acceptor;
};
//}; // Namespace Internal

//    using Internal::Service;
};
