CC=gcc
CFLAGS=-g -Wall -I/usr/include/asm

TARGET=syscalltab_test
OBJS=syscalltab.o
SYSCALL_HEADER=syscalltab.h
BUILD_DIR=build
TEST_DIR=tests
SYSCALL_EXPAND_H=_syscall_expand.h
UNISTD_TEST_H=_unistd_test.h

all: build $(TARGET)

$(TARGET): s.c $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) s.c $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $*.c

clean:
	rm -f *.o $(TARGET) \
                  $(SYSCALL_HEADER) \
                  $(UNISTD_TEST_H) \
                  $(SYSCALL_EXPAND_H) \
                  $(TEST_DIR)/*.{so,h}
	rm -rf $(BUILD_DIR)

test: build build_test
	cp build/lib*/*.so $(SYSCALL_EXPAND_H) tests/
	python tests/sct_tests.py

build_header:
	./syscalltab_gen.sh

build: build_header
	python setup.py build

build_test: expand
expand: $(SYSCALL_EXPAND_H)
$(SYSCALL_EXPAND_H):
	./syscalltab_gen.sh -e

tempfiles: $(SYSCALL_EXPAND_H) $(UNISTD_TEST_H)
$(UNISTD_TEST_H):
	./syscalltab_gen.sh -t
