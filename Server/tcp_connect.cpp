#include "include/tcp_connect.h"


#include <iostream>

namespace Serf {
    tcpConnection::tcpConnection(io::ip::tcp::socket&& socket) : _socket(std::move(socket)) {
        boost::system::error_code ec;
        std::stringstream name;
        name << _socket.remote_endpoint();
        _username = name.str();
        
    }

    void tcpConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {        
        _messageHandler = std::move(messageHandler);      
        _errorHandler = std::move(errorHandler);               
        asyncRead();
    }

    void tcpConnection::Post(const std::string &message) {
      //  bool queueIdle = _outgoingMessages.empty();
        // добавляем наше сообщение 
       // _outgoingMessages.push(message);
        //if (queueIdle) {
            asyncWrite(message);
      //  }
    }

    void tcpConnection::asyncRead() {
        // читаем из сокета поток _streamBuf до "\n" 
        io::async_read_until(_socket, _streamBuf, "\n", 
                            // через лямду передаем колличество байт в метод onRead для получения текста из потока 
                            [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                                self->onRead(ec, bytesTransferred);
        });
    }

    void tcpConnection::onRead(boost::system::error_code ec, size_t bytesTranferred) {
        
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

    void tcpConnection::asyncWrite(const std::string &message) {
        // Записываем наше сообщение в буфер для отправки клиенту front() сколько байт
        io::async_write(_socket, io::buffer(message), 
                        // через лямду записываем наше сообщение 
                        [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred) {
                            self->onWrite(ec, bytesTransferred);
        });
    }

    void tcpConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        if (ec) {
            _socket.close(ec);
            _errorHandler();
            return;
        }
       // _outgoingMessages.pop();
          
       // if (!_outgoingMessages.empty()) {
       //     asyncWrite();
       // }
    }

}