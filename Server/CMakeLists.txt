cmake_minimum_required(VERSION 3.26)

project(server)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS  ON)

ADD_EXECUTABLE(${PROJECT_NAME} server.cpp
                      include/jsonparser.h
                      include/mathparser.h
                      include/tcp_connect.h
                      include/tcp_server.h
                      include/logging.h

                      jsonparser.cpp
                      mathparser.cpp
                      tcp_connect.cpp
                      tcp_server.cpp)

if(WIN32)
    target_link_libraries(server Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log  Boost::property_tree gtest_main wsock32 ws2_32)
else()
    target_link_libraries(server Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log  Boost::property_tree gtest_main)
endif()
