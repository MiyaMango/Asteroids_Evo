CC=g++
CCFLAGS=-c -MMD -MP -std=c++20 -O3 -I../raylib/include 
LDFLAGS=-L../raylib/lib -lraylib -lm

ifeq ($(OS), Windows_NT)
	LDFLAGS+=-lgdi32 -lwinmm
endif

SRC=$(shell find * -name "*.cpp")
OBJ = $(patsubst %.cpp, ../build/%.o, $(SRC))

all: main

run: main
	./main

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

../build/%.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -f main main.exe $(OBJ)