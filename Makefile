CFLAGS = -Wall -pthread
CFILES := src/main.c src/list.c

MKDIR_P = mkdir -p bin

bin/main: $(CFILES) 
	$(MKDIR_P);	
	gcc $(CFLAGS) -o $@ $^
clean:
	rm -rf bin/
run:
	./bin/main

