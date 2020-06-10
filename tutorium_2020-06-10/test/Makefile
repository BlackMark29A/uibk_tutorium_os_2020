CFLAGS = -std=gnu11 -Wall -Werror -Wextra -pedantic

.PHONY: all clean version

all: main

debug: main.c
	$(CC) $(CFLAGS) -Og -g3 -o $@ $<

clean:
	$(RM) main.o main debug

version:
	$(CC) --version

main: main.c
