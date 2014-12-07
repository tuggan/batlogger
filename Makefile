CC=clang
CFLAGS=-c -Wall
LDFLAGS=
EXECUTABLE=batlogger

$(EXECUTABLE): main.c main.h
	$(CC) $(LDFLAGS) main.c -o $@





