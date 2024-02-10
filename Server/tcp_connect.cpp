#include "../lib/logging.h"
#include "include/tcp_connect.h"


#include <iostream>

namespace Serf {
    tcpConnection::tcpConnection(io::ip::tcp::socket&& socket) : _socket(std::move(socket)) {
        boost::system::error_code ec;
        LOGGING_SOURCES(normal, "Конструктор класса tcpConnection");
        std::stringstream name;
        name << _socket.remote_endpoint();
        _username = name.str();
        LOGGING_SOURCES(normal, "Сохраняем имя пользователя " + _username);
        
    }

    void tcpConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {        
        _messageHandler = std::move(messageHandler);      
        _errorHandler = std::move(errorHandler);               
        asyncRead();
    }

    void tcpConnection::Post(const std::string &message) {
        bool queueIdle = _outgoingMessages.empty();
        // добавляем наше сообщение 
        _outgoingMessages.push(message);
        LOGGING_SOURCES(normal, "Добавили наше сообшение в очередь.");
        if (queueIdle) {
            asyncWrite();
        }
    }

    void tcpConnection::asyncRead() {
        LOGGING_SOURCES(normal, "Рекурсия asyncRead");
        // читаем из сокета поток _streamBuf до "\n" 
        io::async_read_until(_socket, _streamBuf, "\n", 
                            // через лямду передаем колличество байт в метод onRead для получения текста из потока 
                            [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                                LOGGING_SOURCES(normal, "Забираем наше сообщение пока не встретим \\n");
                                self->onRead(ec, bytesTransferred);
        });
    }

    void tcpConnection::onRead(boost::system::error_code ec, size_t bytesTranferred) {
        
        if (ec) {
            _socket.close(ec);
            _errorHandler();
            LOGGING_SOURCES(error, "Ошибка.");
            return;
        }
        // забираем из потока наше сообщение        
        std::stringstream message;
        message << _username << ": " << std::istream(&_streamBuf).rdbuf();
        _streamBuf.consume(bytesTranferred);
        LOGGING_SOURCES(normal, "Забираем наше сообщение.");

        // Сохраняем наш хендл и текст 
        _messageHandler(message.str());
        asyncRead();
    }

    void tcpConnection::asyncWrite() {
        LOGGING_SOURCES(normal, "Записываем сообщение.");
        // Записываем наше сообщение в буфер для отправки клиенту front() сколько байт
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), 
                        // через лямду записываем наше сообщение 
                        [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                            LOGGING_SOURCES(normal, "Записываем в _socket.");
                            self->onWrite(ec, bytesTransferred);
        });
    }

    void tcpConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        LOGGING_SOURCES(normal, "Записываем в _socket.");
        if (ec) {
            _socket.close(ec);
            _errorHandler();
            LOGGING_SOURCES(error, "Ошибка.");
            return;
        }
        LOGGING_SOURCES(normal, "Удаление из стека.");  
        _outgoingMessages.pop();
          
        if (!_outgoingMessages.empty()) {
            asyncWrite();
        }
    }

}