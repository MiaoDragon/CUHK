IDIR=./include
CC=gcc
CFLAGS=-I. -std=c99 -Wall -O3 -g
LDFLAGS=-lm -lcunit
OBJ=tester.o manager.o

%.o: %.c $(IDIR)/%.h
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

tester: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf tester
	rm -rf *.o
