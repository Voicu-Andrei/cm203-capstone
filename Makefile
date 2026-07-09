CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -O2
SRC     = main.c cpu.c ui.c programs.c

machine: $(SRC) cpu.h
	$(CC) $(CFLAGS) -o machine $(SRC)

test: machine
	./machine A --run
	./machine B --run

clean:
	rm -f machine machine.exe

.PHONY: test clean
