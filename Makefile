OUTPATH = build
PROJECT = $(OUTPATH)/oscilloscope_gui

CC = g++
CFLAGS=-std=c++11 -Wall -o

.PHONY: dir all clean

all: dir main.cpp TTY.cpp
	$(CC) main.cpp TTY.cpp $(CFLAGS) $(PROJECT)

dir: ${OUTPATH}
${OUTPATH}: 
	mkdir -p ${OUTPATH}

clean: 
	rm -rf $(OUTPATH)

