#include "../lib/logging.h"
#include "include/tcp_server.h"

#include <iostream>

namespace Serf {
    using boost::asio::ip::tcp;

    // Конструктор класса tcpServer
    tcpServer::tcpServer(const IPV ipv, const int port) 
        : _ipVersion(ipv), _port(port),
          _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port)) {
        LOGGING_SOURCES(normal, "Конструктор класса tcpServer");
    }

    // Метод запуска сервера
    int tcpServer::Run() {
        LOGGING_SOURCES(normal, "Запуск");
        try {
            startAccept(); // Начинаем принимать соединения
            _ioContext.run(); // Запускаем цикл обработки событий ввода-вывода
        } catch (const std::exception& e) {
            LOGGING_SOURCES(error, "Ошибка: " + std::string(e.what()));
            std::cerr << e.what() << std::endl; // Выводим информацию об ошибке в консоль
            return -1; // Возвращаем код ошибки
        }
        return 0; // Возвращаем успешный код завершения
    }

// Метод для отправки сообщения всем клиентам
void tcpServer::Broadcast(const std::string& message) {
    // Сортируем и парсим сообщение
    std::string parsedMessage = ParseMessage(message);

    // Получаем результат из формулы
    double result = EvaluateFormula(parsedMessage);

    // Подготавливаем сообщение для отправки
    std::string preparedMessage = PrepareMessage(result);

    // Отправляем сообщение каждому клиенту
    SendMessageToClients(preparedMessage);
}

// Метод для сортировки и парсинга сообщения
std::string tcpServer::ParseMessage(const std::string& message) {
    pars.StringSorting(message);
    LOGGING_SOURCES(normal, "Распарсили наше сообщение.");

    const std::string inkey = "formula";
    return pars.TranslateJsonText(inkey, pars.GetJson());
}

// Метод для вычисления результата формулы
double tcpServer::EvaluateFormula(const std::string& formula) {
    auto it = formula.begin();
    auto sumformula = math.Parse(it, formula.end());
    return sumformula->Evaluate();
}

// Метод для подготовки сообщения для отправки
std::string tcpServer::PrepareMessage(double result) {
    std::string resultStr = std::to_string(result);
    if (result == static_cast<int>(result)) {
        resultStr = resultStr.substr(0, resultStr.find('.'));
    }

    LOGGING_SOURCES(normal, "Сумма : " + resultStr);

    const std::string outkey = "summa";
    return pars.GetId() + ": " + pars.TranslateTextJson(outkey, resultStr) + "\n";
}

void tcpServer::SendMessageToClients(const std::string& message) {
    auto username = pars.GetId();
    auto connection = std::find_if(_connections.begin(), _connections.end(), 
        [username](const std::shared_ptr<tcpConnection>& conn) {
            return username == conn->GetUsername();
        });

    if (connection != _connections.end()) {
        (*connection)->Post(message);
    }
}


    // Метод для начала принятия новых соединений
    void tcpServer::startAccept() {
        LOGGING_SOURCES(normal, "Старт рекурсивного прослушивания.");
        _socket.emplace(_ioContext);

        // Асинхронно принимаем новые соединения
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code& error) {
            LOGGING_SOURCES(normal, "Если новое подключение, добавляем клиента в коллекцию.");
            if (!error) {
                auto connection = tcpConnection::Create(std::move(*_socket)); // Создаем новое соединение

                if (OnJoin) { // Если установлена функция обработки присоединения клиента
                    LOGGING_SOURCES(normal, "Вызов функции обработки нового пользователя.");
                    OnJoin(connection); // Вызываем эту функцию
                }

                _connections.insert(connection); // Добавляем новое соединение в коллекцию клиентов

                // Обработчик сообщений от клиента
                auto massHandler = [this](const std::string& message) {
                    if (OnClientMessage) { // Если установлена функция обработки сообщений от клиента
                        LOGGING_SOURCES(normal, "Вызов функции обработки текста клиентом.");
                        OnClientMessage(message); // Вызываем эту функцию
                    }
                };

                // Обработчик ошибок при соединении с клиентом
                auto errorhandler = [&, weak = std::weak_ptr(connection)]() {
                    if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                        if (OnLeave) { // Если установлена функция обработки отключения клиента
                            LOGGING_SOURCES(normal, "Вызов функции при выходе пользователя.");
                            OnLeave(shared); // Вызываем эту функцию
                        }
                    }
                };

                connection->Start(massHandler, errorhandler); // Начинаем обработку сообщений от клиента
            }

            startAccept(); // Начинаем принимать новые соединения снова
        });
    }
}
