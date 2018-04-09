# variables
CC=gcc
CCFLAGS=`pkg-config --cflags gtk+-3.0`
LLFLAGS=`pkg-config --libs gtk+-3.0`
LIBS=-L/usr/local/lib -lPISCAN -ljpeg -ltiff

ODIR=obj
all:gtktest

gtktest: obj/gtktest.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LLFLAGS) $(LIBS)

obj/gtktest.o: src/gtktest.cpp
	$(CC) -c -o $@ $^ $(CCFLAGS) -w

.PHONY: clean

clean:
	rm -f obj/*.o 

#end
