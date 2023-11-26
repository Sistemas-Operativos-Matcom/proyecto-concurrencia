CFLAGS = -Wall -pthread
CFILES := src/main.c src/list.c

src/main: $(CFILES)
	gcc $(CFLAGS) -o $@ $?

