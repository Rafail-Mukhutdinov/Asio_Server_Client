# CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(Template_Boost_Test)

# Подключите Boost
# asio regex algorithm date_time filesystem system program_options json thread
set(BOOST_INCLUDE_LIBRARIES log log_setup asio regex algorithm date_time filesystem system program_options json thread ) # Укажите нужные вам компоненты Boost
set(BOOST_ENABLE_CMAKE ON)
include(FetchContent)
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z # Укажите нужную вам версию Boost
)
FetchContent_MakeAvailable(Boost)

# Подключите Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Включите тестирование
enable_testing()

# Добавьте ваши директории
add_subdirectory(Client)
add_subdirectory(Server)
#add_subdirectory(Project_2)
#add_subdirectory(Project_3_Qt)
#add_subdirectory(Google_Test)

# Включите Google Test
include(GoogleTest)

