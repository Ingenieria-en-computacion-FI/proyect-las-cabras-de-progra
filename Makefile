CC = gcc
CFLAGS = -Iinclude -Wall
LIBS = -lSDL2

SRC = src/main.c src/render.c src/file_manager.c
TARGET = pacman

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)