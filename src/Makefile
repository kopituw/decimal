# # CC = gcc -Wall -Wextra -Werror -Wpedantic

# ifeq ($(UNAME_S),Linux)
#  OPEN_CMD = xdg-open
#  CFLAGS = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
# endif
# ifeq ($(UNAME_S),Darwin)
#  OPEN_CMD = open
#  CFLAGS = -lcheck -lpthread -lm
# endif

# CC = gcc

# LIB_NAME = s21_decimal
# LIB_FILE = lib$(LIB_NAME).a

# SRC_DIRS = arithmetic help comparison converters other
# C_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
# OBJ_FILES = $(C_FILES:.c=.o)

# TEST_FILE = test_decimal.c
# TEST_BIN = test

# CFLAGS = -Wall -Wextra -Werror -lcheck

# all: clean $(LIB_FILE)

# $(LIB_FILE): $(OBJ_FILES)
# 	ar rcs $@ $^

# %.o: %.c
# 	$(CC) -c $< -o $@

# test: $(LIB_FILE) $(TEST_FILE)
# 	$(CC) $(CFLAGS)  -L. -l$(LIB_NAME) $(TEST_FILE) -o $(TEST_BIN)
# 	./$(TEST_BIN)

# ttest: clean temp_main.c $(LIB_FILE)
# 	$(CC) -lm temp_main.c -o $@ -L. -l$(LIB_NAME)
# 	./ttest


# test: $(LIB_FILE) $(TEST_FILE)
# 	$(CC) $(CFLAGS) $(TEST_FILE) -o $(TEST_BIN) -L. -l$(LIB_NAME)
# 	./$(TEST_BIN)

# clean:
# 	rm -f $(OBJ_FILES) $(LIB_FILE) $(TEST_BIN)

# .PHONY: all clean test


UNAME_S := $(shell uname -s)

# Настройки в зависимости от ОС
ifeq ($(UNAME_S),Linux)
    OPEN_CMD = xdg-open
    TEST_LIBS = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
    GCOV = gcov
endif
ifeq ($(UNAME_S),Darwin)
    OPEN_CMD = open
    # ПРАВИЛЬНЫЕ ПУТИ ДЛЯ macOS
    CHECK_INCLUDE_PATH = /opt/homebrew/Cellar/check/0.15.2/include
    CHECK_LIB_PATH = /opt/homebrew/Cellar/check/0.15.2/lib
    CFLAGS += -I$(CHECK_INCLUDE_PATH)
    TEST_LIBS = -L$(CHECK_LIB_PATH) -lcheck -lpthread -lm
    GCOV = gcov
endif

CC = gcc
BASE_CFLAGS = -Wall -Wextra -Werror -std=c11
CFLAGS ?= $(BASE_CFLAGS)

LIB_NAME = s21_decimal
LIB_FILE = lib$(LIB_NAME).a

SRC_DIRS = arithmetic help comparison converters other
C_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(C_FILES:.c=.o)

TEST_FILE = test_decimal.c
TEST_BIN = test

GCOV_FLAGS = --coverage
GCOV_DIR = gcov_report
GCOV_INFO = $(GCOV_DIR)/coverage.info

all: clean $(LIB_FILE)

$(LIB_FILE): $(OBJ_FILES)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB_FILE) $(TEST_FILE)
	$(CC) $(CFLAGS) $(TEST_FILE) -o $(TEST_BIN) -L. -l$(LIB_NAME) $(TEST_LIBS)
	./$(TEST_BIN)

# Цель для генерации отчета gcov
gcov_report: $(TEST_FILE) $(C_FILES)
	@echo "Compiling with coverage flags..."
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_FILE) $(C_FILES) -o $(TEST_BIN) $(TEST_LIBS)
	@echo "Running tests..."
	./$(TEST_BIN)
	@echo "Generating coverage info..."
	@mkdir -p $(GCOV_DIR)
	lcov -t "s21_decimal" -o $(GCOV_INFO) -c -d . --include "*.c" --exclude "test_*.c"
	@echo "Generating HTML report..."
	genhtml -o $(GCOV_DIR) $(GCOV_INFO)
	@echo "Report generated at: $(GCOV_DIR)/index.html"
	@echo "To open report manually: open $(GCOV_DIR)/index.html"

# Альтернативная цель если lcov не установлен (только gcov)
gcov_simple: $(TEST_FILE) $(C_FILES)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_FILE) $(C_FILES) -o $(TEST_BIN) $(TEST_LIBS)
	./$(TEST_BIN)
	$(GCOV) *.gcno
	@echo "Coverage files generated:"
	@ls -la *.gcov

clean:
	rm -rf $(OBJ_FILES) $(LIB_FILE) $(TEST_BIN) *.gcno *.gcda *.info *.gcov $(GCOV_DIR)

.PHONY: all clean test gcov_report gcov_simple