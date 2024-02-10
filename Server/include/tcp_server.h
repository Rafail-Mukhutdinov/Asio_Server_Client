
#pragma once
#include <boost/asio.hpp>
#include <functional>
#include <optional>
#include <unordered_set>
#include "tcp_connect.h"
#include "../../lib/jsonparser.h"
#include "mathparser.h"


namespace Serf {
    namespace io = boost::asio;

    enum class IPV {
        V4,
        V6
    };



    class tcpServer {        
        using OnJoinHandler = std::function<void(tcpConnection::pointer)>;
        using OnLeaveHandler = std::function<void(tcpConnection::pointer)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        tcpServer(IPV ipv, int port);        

        int Run();
        void Broadcast(const std::string& message);
        
    private:
        void startAccept();

    public:
        OnJoinHandler OnJoin;
        OnLeaveHandler OnLeave;
        OnClientMessageHandler OnClientMessage;

    private:
        IPV _ipVersion;
        int _port;
        
        jsonparser pars;
        mathparse math;
        io::io_context _ioContext;
        io::ip::tcp::acceptor  _acceptor;
        std::optional<io::ip::tcp::socket> _socket;
        std::unordered_set<tcpConnection::pointer> _connections {};
    };
}