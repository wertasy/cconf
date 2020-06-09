CFLAGS += -g -Wall -O0
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean cconf_test rbtree_test

all: rbtree_test cconf_test

clean:
	rm -f $(OBJECTS) cconf_test rbtree_test

rbtree_test: rbtree.o rbtree_test.o
	cc $(CFLAGS) $^ -o $@

cconf_test: rbtree.o cconf.o cconf_test.o
	cc $(CFLAGS) $^ -o $@

%.o: %.c
	cc $(CFLAGS) $^ -c -o $@
