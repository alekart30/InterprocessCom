CC=gcc 
LIBS=-lpthread -lrt 
GAME_FILE=game.c 

CONN_FILES=$(shell find . -name "conn_*.c")

EXECUTABLES=$(CONN_FILES:./conn_%.c=game_%)

all: $(EXECUTABLES)

game_%: conn_%.o $(GAME_FILE:.c=.o)
$(CC) -o $@ $^ $(LIBS) 

clean:
rm -rf *.o $(EXECUTABLES)