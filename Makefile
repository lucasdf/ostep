OUTPUT_DIR = ./build

CFLAGS = -Wall -pthread -g
CINCLUDES = -I./include
CC = gcc

SRCS_C=$(wildcard *.c)
OBJS_C=$(SRCS_C:.c=.o)

JAVAC = javac
SRCS_JAVA=$(wildcard *.java)
OBJS_JAVA=$(SRCS_JAVA:.java=.class)

all:
	rm -f ./compile_commands.json && \
		bear make _all_C \
		&& make _all_java
_all_C: $(OBJS_C)
_all_java: $(OBJS_JAVA)

clean:
	rm -f ./build/*

%.o: %.c
	$(CC) $(CFLAGS) $(CINCLUDES) -o $(OUTPUT_DIR)/$@ $^

%.class: %.java
	$(JAVAC) -d $(OUTPUT_DIR) $^
