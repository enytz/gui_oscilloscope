OUTPATH = build
PROJECT = $(OUTPATH)/oscilloscope_gui

CC = g++
CFLAGS=-std=c++11 -Wall -lsfml-graphics -lsfml-window -lsfml-system -o


.PHONY: dir all clean

all: dir Button.cpp TTY.cpp Screen.cpp Window.cpp main.cpp
	$(CC) main.cpp Button.cpp TTY.cpp Screen.cpp Window.cpp $(CFLAGS) $(PROJECT)

dir: ${OUTPATH}
${OUTPATH}: 
	mkdir -p ${OUTPATH}

clean: 
	rm -rf $(OUTPATH)

