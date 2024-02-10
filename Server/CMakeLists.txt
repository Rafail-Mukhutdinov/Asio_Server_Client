cmake_minimum_required(VERSION 3.27)
project(server)
set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS  ON)

#add_definitions(-D_WIN32_WINNT=0x0601)  # Add this line

ADD_EXECUTABLE(server   server.cpp
                        ../lib/jsonparser.cpp
                        mathparser.cpp
                        tcp_connect.cpp
                        tcp_server.cpp
                        ../lib/jsonparser.h
                        include/mathparser.h
                        include/tcp_connect.h
                        include/tcp_server.h
                        ../lib/logging.h)

if(WIN32)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")
    target_link_libraries(server Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main wsock32 ws2_32)
else()
    target_link_libraries(server Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main)
endif()
