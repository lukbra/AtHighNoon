CC = gcc
CFLAGS = -Iinclude -I"C:/raylib/include" -Wall -Wextra -g
LDFLAGS = -L"C:/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = src/main.c src/game.c
OBJ = main.o game.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
