CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
project(server)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")

# Use CMake variable instead of environment variable
#set (Boost_ROOT "C:/dev/boost_1_83_0/boost_mingw_1120")

# Use correct component names for Boost libraries
find_package(Boost 1.83.0 COMPONENTS date_time filesystem system program_options json log asio array thread property_tree)

include_directories(${Boost_INCLUDE_DIRS})

# Create target 'server'
ADD_EXECUTABLE(${PROJECT_NAME} server.cpp
                      include/jsonparser.h
                      include/mathparser.h
                      include/tcp_connect.h
                      include/tcp_server.h

                      jsonparser.cpp
                      mathparser.cpp
                      tcp_connect.cpp
                      tcp_server.cpp)

# Link the target with libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LOG_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} wsock32 ws2_32)
