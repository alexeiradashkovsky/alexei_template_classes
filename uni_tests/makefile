CC := g++
TEST_INC := ./inc
TEST_SRC := ./src
OBJ_PATH := ./obj
TMPLT := ../templates

ALL_INC := -I$(TEST_INC) -I$(TMPLT)

CFLAGS := -ansi -pedantic -Werror -Wall -std=c++11 $(ALL_INC)

DYN_LIB := -ldl -lpthread

OBJ := $(OBJ_PATH)/thread_pool_test.o $(OBJ_PATH)/uni_test.o


APP_NAME := uni_test

$(APP_NAME) : $(OBJ)
	@echo "$^";$(CC) $(CFLAGS) -o $@ $^ $(DYN_LIB)

$(OBJ_PATH)/%.o : $(TEST_SRC)/%.cpp
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

valgrind: $(APP_NAME)
	valgrind --leak-check=full --show-leak-kinds=all -v ./$(APP_NAME)

clean:
	rm -rf $(OBJ_PATH)/*.o