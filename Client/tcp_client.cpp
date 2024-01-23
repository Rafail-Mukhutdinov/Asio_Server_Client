#include "include/logging.h"
#include <iostream>
#include "include/tcp_client.h"


namespace Serf {

    tcpClient::tcpClient(const std::string &address, int port) : _socket(_ioContext) {
       LOGGING_SOURCES(normal, "Конструктор tcpClient." );
        io::ip::tcp::resolver resolver {_ioContext};
        _endpoints = resolver.resolve(address, std::to_string(port));
    }

    void tcpClient::Run() {
        LOGGING_SOURCES(normal, "Старт." );       
        io::async_connect(_socket, _endpoints, 
                          [this](boost::system::error_code ec, io::ip::tcp::endpoint ep) {                           
                                if (!ec){
                                 LOGGING_SOURCES(error, "Ошибка: ");            
                                asyncRead();
                                }
                            });

        _ioContext.run();
    }

    void tcpClient::Stop() {
       LOGGING_SOURCES(normal, "Выходим из программы." );
        boost::system::error_code ec;
        _socket.close(ec);

        if (ec) {
           LOGGING_SOURCES(error, "Ошибка: ");
        }
    }

    void tcpClient::Post(const std::string &message) {
       LOGGING_SOURCES(normal, "Добавляем сообщение." );
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(message);

        if (queueIdle) {
            asyncWrite();
        }
    }

    void tcpClient::asyncRead() {       
        io::async_read_until(_socket, _streamBuf, "\n", 
                            [this](boost::system::error_code ec, size_t bytesTransferred) {
                                LOGGING_SOURCES(normal, "Передаем сообщение из сокета пока невстретим \\n." );
                                onRead(ec, bytesTransferred);
                            });
    }

    void tcpClient::onRead(boost::system::error_code ec, size_t bytesTransferred) {
       LOGGING_SOURCES(normal, "Читаем сообщение ." );
        if (ec) {
            LOGGING_SOURCES(error, "Ошибка и выход");
            Stop();
            return;
        }

        std::stringstream message;
        message << std::istream{&_streamBuf}.rdbuf(); 
        std::string text = message.str();
        std::string outkey = "summa";    
        pars.StringSorting(text);
        LOGGING_SOURCES(normal, "Распарcим наше сообщение.");        
        std::string stringsumma = pars.TranslateJsonText(outkey, pars.GetJson());       
        std::string finish = pars.GetId()+": " + "Сумма: " + stringsumma + "\n";
        LOGGING_SOURCES(normal, "Подготовили для вывода на консоль : " + finish );

        OnMessage(finish);
        asyncRead();
    }

    void tcpClient::asyncWrite() {
       LOGGING_SOURCES(normal, "Записываем." );
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), 
                        [this](boost::system::error_code ec, size_t bytesTransferred) {
                           
                            onWrite(ec, bytesTransferred);
                        });
    }

    void tcpClient::onWrite(boost::system::error_code ec, size_t bytesTransferred) {      
        if (ec) {
            LOGGING_SOURCES(error, "Ошибка и выход");
            Stop();
            return;
        }

        LOGGING_SOURCES(normal, "Удаление из стека.");  
        _outgoingMessages.pop();

        if (!_outgoingMessages.empty()) {
            asyncWrite();
        }
    }
}