CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
project(client)


set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.82.0 COMPONENTS date_time filesystem system program_options json log asio array thread json_parser ptree ...)


  include_directories(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(${PROJECT_NAME}
                              client.cpp
                              jsonparser.cpp
                              tcp_client.cpp
                              include/jsonparser.h
                              include/tcp_client.h)
                              
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LOG_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})




