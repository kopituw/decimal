FLAGS := -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

BUILD_DIR := build
SRC_DIR := arithmetic comparison conversion help other
SRCS = $(wildcard s21_decimal.c)
SRC_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
TEST_SRCS = tests.c

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))

LIB_NAME = s21_decimal
LIB_FILE = $(LIB_NAME).a


ifeq ($(shell uname), Darwin)
CHECK_LIB = -lcheck -lm -lpthread
else
CHECK_LIB = -lcheck -lm -lsubunit -lpthread -lrt
endif

all: create_dirs $(LIB_FILE) test gcov_report

create_dirs:
	@mkdir -p $(BUILD_DIR) $(foreach dir, $(SRC_DIRS), $(BUILD_DIR)/$(dir))

$(BUILD_DIR)/%.o: %.c | create_dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_FILE): $(OBJS) $(OBJ_FILES)
	ar rcs $@ $^
	ranlib $@

test: $(OBJS) $(OBJ_FILES) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $^ -o $@ $(CHECK_LIB)
	./test

add_fl:
	$(eval CFLAGS += --coverage)
gcov_report: add_fl test
	gcov $(BUILD_DIR)/tests.c
	lcov -t "gcov_report" -o $(BUILD_DIR)/gcov_report.info -c -d $(BUILD_DIR)
	genhtml -o $(BUILD_DIR)/gcov_report $(BUILD_DIR)/gcov_report.info
	open build/gcov_report/index.html

clean:
	rm -rf $(BUILD_DIR) $(LIB_FILE) test gcov_report.info tests.c.gcov
