RESET = \033[0m
RED = \033[31m
GRAY = \033[90m
YELLOW = \033[33m
GREEN = \033[32m

CXX = g++
CXXFLAGS = -Wall -Wextra -I./include -std=c++20
LDFLAGS = -lboost_system -lboost_program_options -lgtest -lgtest_main

OBJ_DIR = obj

SRC_SERVER = src/core/Server.cpp src/core/Client.cpp \
            src/network/NetworkManager.cpp src/network/SocketHandler.cpp \
			src/core/CommandHandler.cpp \
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

# all: make_dir server client gfx tests
all: make_dir server

make_dir:
	@mkdir -p $(OBJ_DIR)/src/core $(OBJ_DIR)/src/network $(OBJ_DIR)/src/utils \
	$(OBJ_DIR)/src/client $(OBJ_DIR)/src/gfx \
	$(OBJ_DIR)/tests/core_tests $(OBJ_DIR)/tests/network_tests $(OBJ_DIR)/tests/utils_tests

server: $(OBJ_SERVER)
	@printf "$(GREEN)Linking server... : $(YELLOW)"
	$(CXX) $(OBJ_SERVER) -o server $(LDFLAGS)
	@printf "\n$(RESET)"

client: $(OBJ_CLIENT)
	@printf "$(GREEN)Linking client... : $(YELLOW)"
	$(CXX) $(OBJ_CLIENT) -o client $(LDFLAGS)
	@printf "\n$(RESET)"

gfx: $(OBJ_GFX)
	@printf "$(GREEN)Linking gfx... : $(YELLOW)"
	$(CXX) $(OBJ_GFX) -o gfx $(LDFLAGS)
	@printf "\n$(RESET)"

tests: $(OBJ_TESTS)
	@printf "$(GREEN)Linking tests... : $(YELLOW)"
	$(CXX) $(OBJ_TESTS) $(filter-out $(OBJ_DIR)/src/main.o, $(OBJ_SERVER)) -o unit_tests $(LDFLAGS)
	@printf "\n$(GREEN)"
	./unit_tests
	@printf "\n$(RESET)"

$(OBJ_DIR)/%.o: %.cpp
	@printf "$(GRAY)Compiling $<... : $(YELLOW)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@printf "$(RED)Cleaning up... : $(YELLOW)"
	rm -rf $(OBJ_DIR)
	@printf "$(RESET)"
	
fclean: clean
	@printf "$(RED)Removing executables... : $(YELLOW)"
	rm -f server client gfx unit_tests
	@printf "\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re client server gfx tests make_dir