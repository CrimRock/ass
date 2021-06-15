CC = g++
SRC = $(wildcard src/*.cpp)
OUT = bin/assc
FLAGS = -Iinclude

default: build install

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

install:
	cp $(OUT) /usr/local/$(OUT)