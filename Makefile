FLAGS = -Wall -pthread -g
INCLUDES = -I./include
OUTPUT_DIR = ./build
CC = gcc

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
all:
	rm ./compile_commands.json && bear make _all
_all: $(OBJS)

clean:
	rm -f ./build/*


%.o: %.c
	$(CC) $(FLAGS) $(INCLUDES) -o $(OUTPUT_DIR)/$@ $^
