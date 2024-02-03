# tests/CMakeLists.txt
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB SRC_SOURCES "../Server/jsonparser.cpp")
#file(GLOB LIB_SOURCES "../Project_2/*.cpp")

add_executable(MyTests ${TEST_SOURCES} ${SRC_SOURCES} )#${LIB_SOURCES})


if(WIN32)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")
    target_link_libraries(MyTests Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main wsock32 ws2_32)
else()
    target_link_libraries(MyTests Boost::asio Boost::regex Boost::algorithm Boost::json Boost::log Boost::property_tree gtest_main)
endif()
# Включите Google Test
include(GoogleTest)

gtest_discover_tests(MyTests)