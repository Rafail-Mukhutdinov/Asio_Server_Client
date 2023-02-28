#include "include/tcp_connect.h"

#include <iostream>

namespace Serf {
    tcpConnection::tcpConnection(io::ip::tcp::socket&& socket) : _socket(std::move(socket)) {
        boost::system::error_code ec;
        //std::cout << "Constructor TCPConnection: " << std::endl;
        std::stringstream name;
        name << _socket.remote_endpoint();
        _username = name.str();
        //std::cout << "_username: " << _username << std::endl;
        
    }

    void tcpConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {
        //std::cout << "Start: " << std::endl;
        _messageHandler = std::move(messageHandler);      
        _errorHandler = std::move(errorHandler);        
        asyncRead();
    }

    void tcpConnection::Post(const std::string &message) {
        bool queueIdle = _outgoingMessages.empty();
        // добавляем наше сообщение 
        _outgoingMessages.push(message);
        //std::cout << "Post: " << std::endl;
        if (queueIdle) {
            asyncWrite();
        }
    }

    void tcpConnection::asyncRead() {
        //std::cout << "asyncRead: " << std::endl;
        // читаем из сокета поток _streamBuf до "\n" 
        io::async_read_until(_socket, _streamBuf, "\n", 
                            // через лямду передаем колличество байт в метод onRead для получения текста из потока 
                            [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                                //std::cout << "Lamda async_read_until: " << std::endl;
                                self->onRead(ec, bytesTransferred);
        });
    }

    void tcpConnection::onRead(boost::system::error_code ec, size_t bytesTranferred) {
        //std::cout << "onRead: " << std::endl;
        if (ec) {
            _socket.close(ec);
            _errorHandler();
            return;
        }
        // забираем из потока наше сообщение        
        std::stringstream message;
        message << _username << ": " << std::istream(&_streamBuf).rdbuf();
        _streamBuf.consume(bytesTranferred);
        

        // Сохраняем наш хендл и текст 
        _messageHandler(message.str());
        asyncRead();
    }

    void tcpConnection::asyncWrite() {
        //std::cout << "asyncWrite: " << std::endl;
        // Записываем наше сообщение в буфер для отправки клиенту front() сколько байт
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), 
                        // через лямду записываем наше сщщбщение 
                        [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                            //std::cout << "Lamda async_write: " << std::endl;
                            self->onWrite(ec, bytesTransferred);
        });
    }

    void tcpConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        //std::cout << "onWrite: " << std::endl;
        if (ec) {
            _socket.close(ec);
            _errorHandler();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty()) {
            asyncWrite();
        }
    }

}