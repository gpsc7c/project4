CC = gcc
CFLAGS = -g -Wall -Wshadow
OBJECTS = statSem.o scanner.o parser.o tree.o IDStack.o
TARGET = statSem

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
statSem.o: statSem.c node.h tree.h IDStack.h parser.h 
	$(CC) $(CFLAGS) -c statSem.c node.h tree.h tree.c ttoken.h parser.c parser.h
parser.o: parser.c parser.h scanner.h ttoken.h node.h tree.h tree.c
	$(CC) $(CFLAGS) -c parser.c parser.h scanner.h ttoken.h node.h tree.h tree.c
IDStack.o: IDStack.c IDStack.h tree.c tree.h
	$(CC) $(CFLAGS) -c IDStack.c IDStack.h tree.c tree.h
scanner.o: scanner.c scanner.h ttoken.h langscan.h
	$(CC) $(CFLAGS) -c scanner.c scanner.h ttoken.h langscan.h
.PHONY: clean
clean:
	rm *.o statSem *.gch
