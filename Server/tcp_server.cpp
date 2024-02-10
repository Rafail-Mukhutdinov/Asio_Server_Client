#include "../lib/logging.h"
#include "include/tcp_server.h"




#include <iostream>

namespace Serf
{
    using boost::asio::ip::tcp;
    tcpServer::tcpServer(IPV ipv, int port) : _ipVersion(ipv), _port(port),
                                              _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
        LOGGING_SOURCES(normal, "Конструктор класса tcpServer");
        
    }

    int tcpServer::Run()
    {
        LOGGING_SOURCES(normal, "Запуск");
        try
        {
            
            startAccept();
            _ioContext.run();
        }
        catch (std::exception &e)
        {
            LOGGING_SOURCES(error, "Ошибка: ");
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
        LOGGING_SOURCES(normal, "Распарcим наше сообщение.");        
        std::string stringformula = pars.TranslateJsonText(inkey, pars.GetJson());
        LOGGING_SOURCES(normal, "Формула: " + stringformula);
        auto sumformula = math.Parse(stringformula.begin(), stringformula.end());        
        std::string change = std::to_string(sumformula->Evaluate());
        LOGGING_SOURCES(normal, "Сумма : " + change);
        std::string finish = pars.GetId()+": " + pars.TranslateTextJson(outkey,change)+ "\n";
        LOGGING_SOURCES(normal, "Подготовили сообщение для отправки клиенту : " + finish);
        
        for (auto &connection : _connections)
        {
            if (pars.GetId() == connection->GetUsername())
                connection->Post(finish);
        }
    }

    void tcpServer::startAccept()
    {
        LOGGING_SOURCES(normal, "Старт рекурсия прослушивание.");
        _socket.emplace(_ioContext);

        // асинхронно ждем соединения клиента
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code &error)
                               {
                                            LOGGING_SOURCES(normal, "Если новое подключение добавляем клиента в коллекцию.");
                                            // создаем нового клиента 
                                            auto connection = tcpConnection::Create(std::move(*_socket));
            

        if (OnJoin) {
            LOGGING_SOURCES(normal, "Указатель на функцию, Если новый пользователь.");
            OnJoin(connection);
        }
        // Добавляем нашего пользователя
        _connections.insert(connection);
        if (!error) {
            auto massHandler = [this](const std::string& message) { 
                                    
                                    if (OnClientMessage) {
                                        LOGGING_SOURCES(normal, "Указатель на функцию обработка текста.");
                                        OnClientMessage(message); 
                                        }                                   
                                };

            auto errorhandler = [&, weak =std::weak_ptr(connection)] {
                                        
                                        if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                                            if (OnLeave) 
                                            {
                                                LOGGING_SOURCES(normal, "Указатель на функцию, Если пользователь покинул сервер.");
                                                OnLeave(shared);
                                            }
                                        }
                                };                   
            connection->Start(massHandler, errorhandler);
        }

        startAccept(); });
    }


}