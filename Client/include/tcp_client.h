#pragma once
#include <boost/asio.hpp>
#include <queue>
#include "../../lib/jsonparser.h"

namespace Serf {
    namespace io = boost::asio;
    using MessageHandler = std::function<void(std::string)>;

    class tcpClient {
    public:
        tcpClient(const std::string& address, int port);

        void Run();
        void Stop();
        void Post(const std::string& message);

    private:
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTransferred);
        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    public:
        MessageHandler OnMessage;

    private:

        jsonparser pars;

        io::io_context _ioContext {};
        io::ip::tcp::socket _socket;

        io::ip::tcp::resolver::results_type _endpoints;

        io::streambuf _streamBuf{65536};
        std::queue<std::string> _outgoingMessages {};
    };
}