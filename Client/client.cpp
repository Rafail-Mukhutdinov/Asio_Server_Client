
#include "include/logging.h"
#include "include/tcp_client.h"
#include <iostream>
#include <thread>

using namespace Serf;

int main(int argc, char* argv[]) {

   LOGGING_INIT();
   LOGGING_SOURCES(normal, "Старт клиент!." );

    tcpClient client {"localhost", 1234};

    jsonparser pars; 
    std::string key = "formula";    

    client.OnMessage = [&](const std::string& message) { 
        LOGGING_SOURCES(normal, "Получаем наше сообщение с результатом: " + message);
        std::cout  << message;
    };
    LOGGING_SOURCES(normal, "Создаем поток для входящих сообщений." );  
    std::thread t{[&client] () { client.Run(); }};

    while(true) {
        std::string message;
        getline(std::cin, message);

        if (message == "\\q") break;
         LOGGING_SOURCES(normal, "Подготавливаем наше сообщение переводим в json." + message);
        client.Post(pars.TranslateTextJson(key, message) + "\n");
    }

    client.Stop();
    t.join();
    LOGGING_SOURCES(normal, "Финиш клиент!." );
    return 0;
}


/*

int main() {

    pars::Parser parser;
    std::string inText;

    std::cin >> inText;

    boost::asio::io_service io_service;
  
    // Создание сокета 
    tcp::socket socket(io_service);
  
    // Соединение 
    socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
  
    // Запрос/сообщение от клиента 
    const string msg = parser.TranslateTextJson(inText) + "\n"; 

    boost::system::error_code error;
    boost::asio::write( socket, boost::asio::buffer(msg), error );
     if( !error ) {
        cout << "Клиент отправил приветственное сообщение!" << endl;
     }  
     else {
        cout << "отправить не удалось: " << error.message() << endl;
     }  
  
    // Получение ответа от сервера  
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
     if( error && error != boost::asio::error::eof ) {
        cout << "получить не удалось: " << error.message() << endl;
     }  
     else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
     }  
     
     return 0;
} 
*/