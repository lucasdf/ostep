FLAGS = -Wall -pthread -g
INCLUDES = ../include
OUTPUT_DIR = ../build

all: t0 t1

clean:
	rm -f ../build/t0 ../build/t1

t0: t0.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/t0 t0.c $(FLAGS)

t1: t1.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/t1 t1.c $(FLAGS)
