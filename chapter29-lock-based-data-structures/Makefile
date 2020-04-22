FLAGS = -Wall -pthread -g
INCLUDES = ../include
OUTPUT_DIR = ../build

all: counter-no-locks counter-with-locks

clean:
	rm -f ../build/counter-no-locks ../build/counter-with-locks ../build/queue ../build/linked-list ../build/hash-table

counter-no-locks: counter-no-locks.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/counter-no-locks counter-no-locks.c $(FLAGS)

counter-with-locks: counter-with-locks.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/counter-with-locks counter-with-locks.c $(FLAGS)

queue: queue.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/queue queue.c $(FLAGS)

linked-list: linked-list.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/linked-list linked-list.c $(FLAGS)

hash-table: hash-table.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/hash-table hash-table.c $(FLAGS)

