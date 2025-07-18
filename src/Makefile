# CC = gcc -Wall -Wextra -Werror -Wpedantic

ifeq ($(UNAME_S),Linux)
 OPEN_CMD = xdg-open
 CFLAGS = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
 OPEN_CMD = open
 CFLAGS = -lcheck -lpthread -lm
endif

CC = gcc

LIB_NAME = s21_decimal
LIB_FILE = lib$(LIB_NAME).a

SRC_DIRS = arithmetic help comparison
C_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(C_FILES:.c=.o)

TEST_FILE = test_decimal.c
TEST_BIN = test

CFLAGS = -Wall -Wextra -Werror

all: clean $(LIB_FILE)

$(LIB_FILE): $(OBJ_FILES)
	ar rcs $@ $^

%.o: %.c
	$(CC) -c $< -o $@

test: $(LIB_FILE) $(TEST_FILE)
	$(CC) $(CFLAGS)  -L. -l$(LIB_NAME) $(TEST_FILE) -o $(TEST_BIN)
	./$(TEST_BIN)

ttest: clean temp_main.c $(LIB_FILE)
	$(CC) -lm temp_main.c -o $@ -L. -l$(LIB_NAME)
	./ttest


clean:
	rm -f $(OBJ_FILES) $(LIB_FILE) $(TEST_BIN)

.PHONY: all clean test