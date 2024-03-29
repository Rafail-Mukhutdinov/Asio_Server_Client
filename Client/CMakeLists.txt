cmake_minimum_required(VERSION 3.27)
project(client)
set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS  ON)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")

add_executable(client   client.cpp
                        ../lib/jsonparser.cpp
                        tcp_client.cpp
                        ../lib/jsonparser.h
                        include/tcp_client.h
                        ../lib/logging.h)

#target_link_libraries(client PRIVATE Qt6::Widgets  ${Boost_LOG_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} wsock32 ws2_32)
if(WIN32)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")
    target_link_libraries(client Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main wsock32 ws2_32)
else()
    target_link_libraries(client Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main)
endif()
