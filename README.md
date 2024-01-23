[![CMake on multiple platforms](https://github.com/Rafail-Mukhutdinov/Asio_Server_Client/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Rafail-Mukhutdinov/Asio_Server_Client/actions/workflows/cmake-multi-platform.yml)

# Asio_Server_Client

* Wrote code on OS Linux Centos 9 stream
* Building projects through Cmake
* Requires installation of Boost library from version 1.80
* The code uses asynchronous data transfer via the TCP/IP protocol

## Server:
* The server can accept an unlimited number of clients
* Each client is assigned an ID and saved in the stack if the client left it is removed from the stack
* The server processes incoming messages in json format `{"formula":"5+7/3-7"}` and after calculations, it forms it back into json format `{"summa":"0"}`
* On the server, I process mathematical operations if division by zero occurs this operation I exclude for example 2+2/0 outputs the result 2 (2/0) is excluded
* I use a stack for the priority of mathematical operations
* Logging is handled by a global macro in the log output I added a parameter, function name, file name, line number, and message

## Client
* The client uses multithreading to receive data from the server
* Everything is the same as the server only there is no calculation of formulas
* Exit from the client input to the console



  
##

  
![Снимок экрана от 2023-03-03 12-47-30](https://user-images.githubusercontent.com/22080035/222641664-8a213c3f-0645-4e53-ab38-60ab9fd14ffc.png)
