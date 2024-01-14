# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -pedantic

# Object files for the library
OBJS = src/rb_tree.o src/rb_tree_delete.o src/rb_tree_destroy.o src/rb_tree_find.o src/rb_tree_insert.o src/rb_tree_new.o src/rb_tree_utils.o

OBJS_TESTS = tests/rb_tree_tests.o $(OBJS)

# Rule to make the library
all: CFLAGS += -O3
all: $(OBJS)
	ar rcs librb_tree.a $(OBJS)

debug: CFLAGS += -fsanitize=address -lcriterion -g
debug: $(OBJS_TESTS)
	$(CC) $(CFLAGS) -o main $(OBJS_TESTS)
	
clean:
	rm -f $(OBJS) main tree.dot

clean_debug:
	rm -f $(OBJS_TESTS) main 


