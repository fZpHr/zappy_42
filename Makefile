RESET = \033[0m
RED = \033[31m
GRAY = \033[90m
YELLOW = \033[33m
GREEN = \033[32m

CXX = g++
CXXFLAGS = -Wall -Wextra -I./include -std=c++20 -g
LDFLAGS = -lboost_system -lboost_program_options -lgtest -lgtest_main

OBJ_DIR = obj

SRC_SERVER = src/core/Server.cpp src/core/Client.cpp \
			src/core/Map.cpp src/core/Team.cpp \
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

valgrind-tests: unit_tests
	@printf "$(GREEN)Running tests with Valgrind... $(RESET)\n"
	@rm -f valgrind_*.log
	@./unit_tests --gtest_list_tests | grep -v "^  " | grep -v "^$$" | while read -r test_suite; do \
		./unit_tests --gtest_list_tests --gtest_filter="$$test_suite*" | grep "^  " | sed "s/^  //" | while read -r test_name; do \
		full_test="$$test_suite$$test_name"; \
		printf "\n$(YELLOW)Running Valgrind on test: $(GREEN)$$full_test$(RESET)\n"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes \
		--log-file="valgrind_$${full_test}.log" ./unit_tests --gtest_filter="$$full_test"; \
		printf "\n$(YELLOW)Valgrind summary for $(GREEN)$$full_test$(RESET):\n"; \
		printf "$(GRAY)"; \
		grep -A1 "ERROR SUMMARY\|LEAK SUMMARY\|FILE DESCRIPTORS\|definitely lost\|indirectly lost\|Invalid \(read\|write\)\|uninitialised\|Use of uninitialised" "valgrind_$${full_test}.log"; \
		printf "$(RESET)\n"; \
		cat "valgrind_$${full_test}.log"; \
		error_count=$$(grep "ERROR SUMMARY" "valgrind_$${full_test}.log" | sed -E 's/.*ERROR SUMMARY: ([0-9,]+) errors.*/\1/'); \
		invalid_reads=$$(grep -c "Invalid read" "valgrind_$${full_test}.log" || echo "0"); \
		invalid_writes=$$(grep -c "Invalid write" "valgrind_$${full_test}.log" || echo "0"); \
		uninit_vals=$$(grep -c "uninitialised" "valgrind_$${full_test}.log" || echo "0"); \
		fd_count=$$(grep "FILE DESCRIPTORS:" "valgrind_$${full_test}.log" | sed -E 's/.*FILE DESCRIPTORS: ([0-9]+) open.*/\1/'); \
			if [ -n "$$fd_count" ] && [ $$fd_count -gt 4 ]; then \
				fd_leaks=$$((fd_count - 4)); \
			else \
				fd_leaks=0; \
			fi; \
		def_leaks_line=$$(grep "definitely lost:" "valgrind_$${full_test}.log"); \
		def_leaks=$$(echo $$def_leaks_line | sed -E 's/.*definitely lost: ([0-9,]+) bytes.*/\1/' || echo "0"); \
		ind_leaks_line=$$(grep "indirectly lost:" "valgrind_$${full_test}.log"); \
		ind_leaks=$$(echo $$ind_leaks_line | sed -E 's/.*indirectly lost: ([0-9,]+) bytes.*/\1/' || echo "0"); \
		printf "$(YELLOW)Error summary for $(GREEN)$$full_test$(RESET):\n"; \
		if [ "$$error_count" = "0" ]; then \
				printf "$(GREEN)✓ No Valgrind errors detected$(RESET)\n"; \
			else \
				printf "$(RED)✗ Total errors: $$error_count$(RESET)\n"; \
			fi; \
			if [ "$$invalid_reads" != "0" ]; then \
				printf "$(RED)✗ Invalid reads: $$invalid_reads$(RESET)\n"; \
			fi; \
			if [ "$$invalid_writes" != "0" ]; then \
				printf "$(RED)✗ Invalid writes: $$invalid_writes$(RESET)\n"; \
			fi; \
			if [ "$$uninit_vals" != "0" ]; then \
				printf "$(RED)✗ Uninitialized values: $$uninit_vals$(RESET)\n"; \
			fi; \
			if [ "$$fd_leaks" != "0" ]; then \
				printf "$(RED)✗ File descriptor leaks: $$fd_leaks$(RESET)\n"; \
			fi; \
			if [ -z "$$def_leaks" ] || [ "$$def_leaks" = "0" ]; then \
				printf "$(GREEN)✓ No direct memory leaks$(RESET)\n"; \
			else \
				printf "$(RED)✗ Direct memory leaks: $$def_leaks bytes$(RESET)\n"; \
			fi; \
			if [ -z "$$ind_leaks" ] || [ "$$ind_leaks" = "0" ]; then \
				printf "$(GREEN)✓ No indirect memory leaks$(RESET)\n"; \
			else \
				printf "$(RED)✗ Indirect memory leaks: $$ind_leaks bytes$(RESET)\n"; \
			fi; \
		done; \
	done
	@printf "\n$(GREEN)Final Valgrind summary:$(RESET)\n"
	@grep -l "ERROR SUMMARY: 0 errors" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(GREEN)Tests with no errors: %s$(RESET)\n"
	@grep -l "ERROR SUMMARY: [1-9]" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(RED)Tests with errors: %s$(RESET)\n"
	@grep -l "definitely lost: 0 bytes" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(GREEN)Tests with no memory leaks: %s$(RESET)\n"
	@grep -l "definitely lost: [1-9]" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(RED)Tests with memory leaks: %s$(RESET)\n"
	@grep -l "Invalid read" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(RED)Tests with invalid reads: %s$(RESET)\n"
	@grep -l "Invalid write" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(RED)Tests with invalid writes: %s$(RESET)\n"
	@grep -l "Use of uninitialised" valgrind_*.log 2>/dev/null | wc -l | xargs printf "$(RED)Tests with uninitialized values: %s$(RESET)\n"
	@(for file in valgrind_*.log; do \
		fd_count=$$(grep "FILE DESCRIPTORS:" "$$file" | sed -E 's/.*FILE DESCRIPTORS: ([0-9]+) open.*/\1/'); \
		if [ -n "$$fd_count" ] && [ $$fd_count -gt 4 ]; then \
			echo "$$file"; \
		fi; \
	done) | wc -l | xargs printf "$(RED)Tests with FD leaks: %s$(RESET)\n"
	@printf "\n$(GREEN)All tests completed with Valgrind$(RESET)\n"

unit_tests: $(OBJ_TESTS)
	@printf "$(GREEN)Linking tests... : $(YELLOW)"
	$(CXX) $(OBJ_TESTS) $(filter-out $(OBJ_DIR)/src/main.o, $(OBJ_SERVER)) -o unit_tests $(LDFLAGS)
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