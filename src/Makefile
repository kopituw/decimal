# CC = gcc -Wall -Wextra -Werror -Wpedantic
CC = gcc

LIB_NAME = s21_decimal
LIB_FILE = lib$(LIB_NAME).a


SRC_DIRS = arithmetic help comparison
C_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(C_FILES:.c=.o)

all:clean

test: clean temp_main.c $(LIB_FILE)
	$(CC) $(CFLAGS) temp_main.c -o $@ -L. -l$(LIB_NAME)
	./test


test_gcov: clean temp_main.c $(LIB_FILE)
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -lcheck -lpthread -lm test_for_denya.c -o $@ -L. -l$(LIB_NAME)
	./test_gcov
	mkdir -p logs
	gcovr --html-details -e "test_for_denya.c" -o logs/report.html
	$(OPEN_CMD) logs/report.html


$(LIB_FILE): $(OBJ_FILES)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJ_FILES) $(LIB_FILE) test

.PHONY: all clean