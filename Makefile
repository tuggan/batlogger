CC=clang
CFLAGS=-c -Wall
LDFLAGS=
EXECUTABLE=batlogger

$(EXECUTABLE): main.c main.h
	$(CC) $(LDFLAGS) main.c -o $@

plot: plot.plt
	gnuplot plot.plt



