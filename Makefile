CC=gcc
CFLAGS=-Iinclude
DEPS = include/library.h include/partition.h include/structures.h
OBJ = src/library.o src/partition.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o *~ core *~