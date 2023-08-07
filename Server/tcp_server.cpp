#include "include/tcp_server.h"




#include <iostream>

namespace Serf
{
    using boost::asio::ip::tcp;
    tcpServer::tcpServer(IPV ipv, int port) : _ipVersion(ipv), _port(port),
                                              _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
        
    }

    int tcpServer::Run()
    {
        try
        {
            
            startAccept();
            _ioContext.run();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }
    // Работаем стекстом 
    void tcpServer::Broadcast(const std::string &message)
    {
        
        std::string inkey = "formula"; 
        std::string outkey = "summa";    
        pars.StringSorting(message);
        std::string stringformula = pars.TranslateJsonText(inkey, pars.GetJson());
        auto sumformula = math.Parse(stringformula.begin(), stringformula.end());        
        std::string change = std::to_string(sumformula->Evaluate());
        std::string finish = pars.GetId()+": " + pars.TranslateTextJson(outkey,change)+ "\n";
        
        for (auto &connection : _connections)
        {
            if (pars.GetId() == connection->GetUsername())
                connection->Post(finish);
        }
    }

    void tcpServer::startAccept()
    {
        _socket.emplace(_ioContext);

        // асинхронно ждем соединения клиента
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code &error)
                               {
                                            // создаем нового клиента 
                                            auto connection = tcpConnection::Create(std::move(*_socket));
            

        if (OnJoin) {
            OnJoin(connection);
        }
        // Добавляем нашего пользователя
        _connections.insert(connection);
        if (!error) {
            auto massHandler = [this](const std::string& message) { 
                                    
                                    if (OnClientMessage) {
                                        OnClientMessage(message); 
                                        }                                   
                                };

            auto errorhandler = [&, weak =std::weak_ptr(connection)] {
                                        
                                        if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                                            if (OnLeave) 
                                            {
                                                OnLeave(shared);
                                            }
                                        }
                                };                   
            connection->Start(massHandler, errorhandler);
        }

        startAccept(); });
    }


}