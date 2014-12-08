CC=clang
CFLAGS=-c -Wall 
LDFLAGS=
EXECUTABLE=batlogger

all: $(EXECUTABLE)

.PHONY: debug

debug: CFLAGS += -g -DDEBUG 
debug: $(EXECUTABLE)

profile: CFLAGS += -pg
profile: debug

$(EXECUTABLE): main.o
	$(CC) $(LDFLAGS) main.o -o $@

main.o: main.c main.h
	$(CC) $(CFLAGS) main.c -o $@

.PHONY: plot clean
plot: plot.plt
	gnuplot plot.plt

clean:
	rm -rf *.o



