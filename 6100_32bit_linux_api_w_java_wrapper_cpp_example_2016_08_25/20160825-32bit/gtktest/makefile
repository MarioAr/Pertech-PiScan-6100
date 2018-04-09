# variables
CC=gcc
CCFLAGS=`pkg-config --cflags gtk+-3.0`
LLFLAGS=`pkg-config --libs gtk+-3.0`
LIBS=-L/usr/local/lib -lPISCAN

ODIR=obj
all:gtktest

gtktest: obj/gtktest.o
	$(CC) -o $@ $^ $(LIBS)

obj/gtktest.o: src/scan.cpp
	$(CC) -c -o $@ $^

.PHONY: clean

clean:
	rm -f obj/*.o 

#end
