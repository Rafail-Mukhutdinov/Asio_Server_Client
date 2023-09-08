cmake_minimum_required(VERSION 3.16)

project(client)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS  ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Boost 1.83.0 COMPONENTS date_time filesystem system program_options json log thread )
include_directories(${Boost_INCLUDE_DIRS})

add_executable(client   client.cpp
                        jsonparser.cpp
                        tcp_client.cpp
                        include/jsonparser.h
                        include/tcp_client.h)

target_link_libraries(client PRIVATE Qt6::Widgets  ${Boost_LOG_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} wsock32 ws2_32)
