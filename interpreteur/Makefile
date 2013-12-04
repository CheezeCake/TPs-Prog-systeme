CC=cc
CFLAGS=-Wall -ansi -pedantic
TARGET=myshell

all: main.o command.o shell.o
	$(CC) $^ -o $(TARGET)

main.o: main.c
	$(CC) -c $^ $(CFLAGS)

%.o: %.c %.h
	$(CC) -c $< $(CFLAGS)

.PHONY: clean mrproper rebuild

clean:
	@rm -f *.o

mrproper: clean
	@rm -f $(TARGET)

rebuild: clean all
