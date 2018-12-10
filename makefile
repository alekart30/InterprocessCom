CC=gcc 
CFLAGS=-Wall -O3
LIBS=-lpthread -lrt 
GAME_FILE=game.c 

CONN_FILES=$(shell find . -name "conn_*.c")

EXECUTABLES=$(CONN_FILES:./conn_%.c=game_%)

all: $(EXECUTABLES)

game_%: conn_%.o $(GAME_FILE:.c=.o)
$(CC) -o $@ $^ $(LIBS) 

.c.o:
$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean:
rm -f `find . -maxdepth 1 -executable -type f`