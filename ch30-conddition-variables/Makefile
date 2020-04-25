FLAGS = -Wall -pthread -g
INCLUDES = ../include
OUTPUT_DIR = ../build

all: spin-waiting-conditional conditional-variable flawed-conditional-without-loop-check \
flawed-conditional-without-lock

clean:
	rm -f ../build/spin-waiting-conditional

spin-waiting-conditional: spin-waiting-conditional.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/spin-waiting-conditional spin-waiting-conditional.c $(FLAGS)

conditional-variable: conditional-variable.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/conditional-variable conditional-variable.c $(FLAGS)

flawed-conditional-without-loop-check: flawed-conditional-without-loop-check.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/flawed-conditional-without-loop-check flawed-conditional-without-loop-check.c $(FLAGS)

flawed-conditional-without-lock: flawed-conditional-without-lock.c
	gcc -I $(INCLUDES) -o $(OUTPUT_DIR)/flawed-conditional-without-lock flawed-conditional-without-lock.c $(FLAGS)
