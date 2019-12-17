CC := g++
TEST_INC := ./inc
TEST_SRC := ./src
OBJ_PATH := ./obj
PYTHON_3_6_INC_PATH := /usr/include/python3.6m

ALL_INC := -I$(TEST_INC) -I$(PYTHON_3_6_INC_PATH)

CFLAGS := -ansi -pedantic -Werror -Wall -std=c++11 $(ALL_INC)

DYN_LIB := -ldl
DYN_LIB += -lpthread
DYN_LIB += -lutil
DYN_LIB += -lm
DYN_LIB += -Xlinker
DYN_LIB += -export-dynamic
# DYN_LIB += -lpython3.6m

OBJ := $(OBJ_PATH)/thread_pool_test.o
OBJ += $(OBJ_PATH)/creational_design_patterns.o
OBJ += $(OBJ_PATH)/uni_test.o
# OBJ += $(OBJ_PATH)/python_test.o


APP_NAME := uni_test

$(APP_NAME) : $(OBJ)
	@echo "$^";$(CC) $(CFLAGS) -o $@ $^ $(DYN_LIB)

$(OBJ_PATH)/%.o : $(TEST_SRC)/%.cpp $(TMPLT)/*
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

valgrind: $(APP_NAME)
	valgrind --leak-check=full --show-leak-kinds=all -v ./$(APP_NAME)

clean:
	rm -rf $(OBJ_PATH)/*.o