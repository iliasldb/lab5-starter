all: number-server

number-server: number-server.c http-server.h
	gcc -o number-server number-server.c http-server.c -lpthread -std=c99 -Wall -Wno-unused-variable

clean:
	rm -f number-server

