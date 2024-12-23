cmake_minimum_required(VERSION 3.10)
project(Zappy)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(zappy_lib STATIC
   src/core/Server.cpp
   src/core/Client.cpp
   src/network/NetworkManager.cpp
   src/network/SocketHandler.cpp
   src/utils/Logger.cpp
)

add_executable(zappy_server src/main.cpp)
target_link_libraries(zappy_server PRIVATE zappy_lib)

add_executable(zappy_tests
   tests/mainTest.cpp
   tests/core_tests/ClientTest.cpp
   tests/core_tests/ServerTest.cpp
   tests/network_tests/SocketHandlerTest.cpp
   tests/network_tests/NetworkManagerTest.cpp
   tests/utils_tests/LoggerTest.cpp
   tests/utils_tests/ExceptionTest.cpp
)

target_link_libraries(zappy_lib PRIVATE 
   Boost::system 
   ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(zappy_tests PRIVATE
   zappy_lib
   GTest::GTest
   GTest::Main
   Boost::system
   ${CMAKE_THREAD_LIBS_INIT}
)

add_custom_target(hot_reload
   COMMAND ${CMAKE_COMMAND} -E env watchexec -w src -e cpp,hpp -- "cmake --build . && ./zappy_server 4242"
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

enable_testing()
gtest_discover_tests(zappy_tests)