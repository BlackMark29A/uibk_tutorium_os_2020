CFLAGS = -std=gnu11 -Wall -Werror -Wextra -Wconversion -pedantic -O3
LDFLAGS = -lpthread

.PHONY: all clean

all: main debug

debug: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -Og -g3 -o $@ $<

main: main.c

clean:
	$(RM) main debug
