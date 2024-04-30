CC = gcc
CFLAGS = -g -Wall -Wshadow
OBJECTS = initializer.o scanner.o parser.o tree.o IDStack.o generator.o
TARGET = compfs

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
initializer.o: initializer.c node.h tree.h IDStack.h parser.h 
	$(CC) $(CFLAGS) -c initializer.c node.h tree.h tree.c ttoken.h parser.c parser.h
parser.o: parser.c parser.h scanner.h ttoken.h node.h tree.h tree.c
	$(CC) $(CFLAGS) -c parser.c parser.h scanner.h ttoken.h node.h tree.h tree.c
IDStack.o: IDStack.c IDStack.h tree.c tree.h
	$(CC) $(CFLAGS) -c IDStack.c IDStack.h tree.c tree.h
scanner.o: scanner.c scanner.h ttoken.h langscan.h
	$(CC) $(CFLAGS) -c scanner.c scanner.h ttoken.h langscan.h
generator.o: generator.c generator.h tree.h IDStack.h
	$(CC) $(CFLAGS) -c generator.c generator.h
.PHONY: clean
clean:
	rm *.o compfs *.gch
