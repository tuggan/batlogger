CC=clang
CFLAGS=-c -Wall
LDFLAGS=
EXECUTABLE=batlogger
OBJ = main.o fs.o battery.o

all: $(EXECUTABLE)

debug: CFLAGS += -g -DDEBUG 
debug: $(EXECUTABLE)

profile: CFLAGS += -pg
profile: debug

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: plot clean
plot: plot.plt
	gnuplot plot.plt

clean:
	rm -rf *.o



