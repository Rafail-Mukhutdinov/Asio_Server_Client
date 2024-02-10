#include <iostream> 
#include "include/tcp_server.h" 
#include "../lib/logging.h"



int main(int argc, char* argv[]) {

    LOGGING_INIT();
    // Параметры логирования // normal, notification, warning, error, critical
    LOGGING_SOURCES(normal, "Старт Сервера!");

    Serf::tcpServer server {Serf::IPV::V4, 1234};

    server.OnJoin = [](Serf::tcpConnection::pointer server) {
        std::cout << "Пользователь присоединился к серверу: " << server->GetUsername() << std::endl;
        LOGGING_SOURCES(notification, "Пользователь присоединился к серверу: " + server->GetUsername());
    };

    server.OnLeave = [](Serf::tcpConnection::pointer server) {
        std::cout << "Пользователь покинул сервер: " << server->GetUsername() << std::endl;
        LOGGING_SOURCES(notification, "Пользователь покинул сервер: " + server->GetUsername());
    };

    server.OnClientMessage = [&server](const std::string& message) {
        LOGGING_SOURCES(notification, "Пришло сообщение от : " + message);
        // Обрабатываем сообщения 
        server.Broadcast(message);
    };

    server.Run();
    LOGGING_SOURCES(normal, "Финиш Сервера!");
    return 0;
}

/*


#include <boost/asio.hpp>  
  
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;



string read_(tcp::socket & socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until( socket, buf, "\n" );
    string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
}  
  
void send_(tcp::socket & socket, const string& message) {
    const string msg = message + "\n";
    boost::asio::write( socket, boost::asio::buffer(message) );
} 


boost::asio::io_service io_service;
  
// Слушаем новое соединение 
tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234 ));
  
// Создание сокета 
tcp::socket socket_(io_service);
  
// Ожидаем  соединения  
acceptor_.accept(socket_);
  
// Читаем из сокета 
string message = read_(socket_);
cout << message << endl;
  
//операция записи  
send_(socket_, "Привет с Сервера!");
cout << "Сервер отправил приветственное сообщение клиенту!" << endl;
      return 0;

      */