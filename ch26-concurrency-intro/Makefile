FLAGS = -Wall -pthread -g
INCLUDES = ../include
OUTPUT_DIR = ../build

all: t0 t1 t1java t1fixedjava t2

clean:
	rm -f ../build/t0 ../build/t1

t0: t0.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/t0 t0.c $(FLAGS)

t1: t1.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/t1 t1.c $(FLAGS)

t1java: t1.java
	javac -d $(OUTPUT_DIR) t1.java

t1fixedjava: t1.java
	javac -d $(OUTPUT_DIR) t1fixed.java

t2: t2.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/t2 t2.c $(FLAGS)
