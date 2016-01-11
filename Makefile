UNAME := $(shell uname)
SHELL := /bin/bash
CC = g++
SOURCES = main.cc
ifeq ($(UNAME), Linux)
	LDFLAGS += -lOpenCL
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS += -framework OpenCL
endif

OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = main

all: kernel $(SOURCES) $(EXECUTABLE)

kernel: horn_schunck.py
	python horn_schunck.py

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm $(OBJECTS) $(EXECUTABLE)
