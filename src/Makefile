# CC = gcc -Wall -Wextra -Werror -Wpedantic
CC = gcc

LIB_NAME = s21_decimal
LIB_FILE = lib$(LIB_NAME).a

SRC_DIRS = arithmetic help comparison
C_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(C_FILES:.c=.o)

TEST_FILE = test_decimal.c
TEST_BIN = test

CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: clean $(LIB_FILE)

$(LIB_FILE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB_FILE) $(TEST_FILE)
	$(CC) $(CFLAGS) $(TEST_FILE) -o $(TEST_BIN) -L. -l$(LIB_NAME)
	./$(TEST_BIN)

clean:
	rm -f $(OBJ_FILES) $(LIB_FILE) $(TEST_BIN)

.PHONY: all clean test