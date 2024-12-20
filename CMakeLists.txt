cmake_minimum_required(VERSION 3.10)
project(ZappyServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options de compilation
option(BUILD_TESTS "Build tests" ON)

# Trouver les dépendances
find_package(Boost REQUIRED COMPONENTS system)
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
endif()

# Inclure les dossiers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Définir les sources
set(SOURCES
    src/core/Server.cpp
    src/core/Client.cpp
    src/network/NetworkManager.cpp
    src/network/SocketHandler.cpp
    src/utils/Logger.cpp
    src/utils/Exception.cpp
)

# Créer la bibliothèque
add_library(zappy_lib STATIC ${SOURCES})
target_link_libraries(zappy_lib PRIVATE Boost::system)

# Créer l'exécutable
add_executable(zappy_server src/main.cpp)
target_link_libraries(zappy_server PRIVATE zappy_lib)

# Tests
if(BUILD_TESTS)
    enable_testing()
    
    set(TEST_SOURCES
        tests/core/ServerTest.cpp
        tests/core/ClientTest.cpp
        tests/network/NetworkTest.cpp
    )
    
    add_executable(zappy_tests ${TEST_SOURCES})
    target_link_libraries(zappy_tests
        PRIVATE
        zappy_lib
        GTest::GTest
        GTest::Main
    )
    
    add_test(NAME zappy_tests COMMAND zappy_tests)
endif()