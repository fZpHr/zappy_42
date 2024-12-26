CXX = g++
CXXFLAGS = -Wall -Wextra -I./include -std=c++17
LDFLAGS = -lboost_system -lpthread -lgtest -lgtest_main

OBJ_DIR = obj

SRC_SERVER = src/core/Server.cpp src/core/Client.cpp \
            src/network/NetworkManager.cpp src/network/SocketHandler.cpp \
            src/utils/Logger.cpp src/main.cpp

SRC_CLIENT = src/client/main.cpp
SRC_GFX = src/gfx/main.cpp

SRC_TESTS = tests/mainTest.cpp \
           tests/core_tests/ClientTest.cpp \
           tests/core_tests/ServerTest.cpp \
           tests/network_tests/SocketHandlerTest.cpp \
           tests/network_tests/NetworkManagerTest.cpp \
           tests/utils_tests/LoggerTest.cpp \
           tests/utils_tests/ExceptionTest.cpp

OBJ_SERVER = $(addprefix $(OBJ_DIR)/, $(SRC_SERVER:.cpp=.o))
OBJ_CLIENT = $(addprefix $(OBJ_DIR)/, $(SRC_CLIENT:.cpp=.o))
OBJ_GFX = $(addprefix $(OBJ_DIR)/, $(SRC_GFX:.cpp=.o))
OBJ_TESTS = $(addprefix $(OBJ_DIR)/, $(SRC_TESTS:.cpp=.o))

all: make_dir server client gfx tests

make_dir:
	@mkdir -p $(OBJ_DIR)/src/core $(OBJ_DIR)/src/network $(OBJ_DIR)/src/utils \
	$(OBJ_DIR)/src/client $(OBJ_DIR)/src/gfx \
	$(OBJ_DIR)/tests/core_tests $(OBJ_DIR)/tests/network_tests $(OBJ_DIR)/tests/utils_tests

server: $(OBJ_SERVER)
	$(CXX) $(OBJ_SERVER) -o server $(LDFLAGS)

client: $(OBJ_CLIENT)
	$(CXX) $(OBJ_CLIENT) -o client $(LDFLAGS)

gfx: $(OBJ_GFX)
	$(CXX) $(OBJ_GFX) -o gfx $(LDFLAGS)

tests: $(OBJ_TESTS)
	$(CXX) $(OBJ_TESTS) $(filter-out $(OBJ_DIR)/src/main.o, $(OBJ_SERVER)) -o unit_tests $(LDFLAGS)
	./unit_tests

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f server client gfx unit_tests

re: fclean all

.PHONY: all clean fclean re client server gfx tests make_dir