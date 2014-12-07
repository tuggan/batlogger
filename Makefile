CC=clang
CFLAGS=-c -Wall
LDFLAGS=
EXECUTABLE=batlogger

all: $(EXECUTABLE)

$(EXECUTABLE): main.c main.h
	$(CC) $(LDFLAGS) main.c -o $@

plot: plot.plt
	gnuplot plot.plt



