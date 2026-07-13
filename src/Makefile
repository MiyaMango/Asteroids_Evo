CC=g++
CCFLAGS=-c -MMD -MP -std=c++20 -O3 -I../raylib/include 
LDFLAGS=-L../raylib/lib -lm

ifeq ($(OS), Windows_NT)
	LDFLAGS+=-lraylib -lgdi32 -lwinmm
	EXE=main.exe
else
	LDFLAGS+=-lraylib_linux -lGL -lm -lpthread -ldl -lrt -lX11
	EXE=main
endif

SRC=$(shell find . -name "*.cpp")
OBJ = $(patsubst ./%.cpp, ../build/%.o, $(SRC))

all: $(EXE)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

../build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<