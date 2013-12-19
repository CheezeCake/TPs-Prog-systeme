CC=cc
CFLAGS=-Wall -pedantic
TARGET=myshell

all: $(TARGET)

$(TARGET): main.o command.o shell.o
	$(CC) $^ -o $@

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
