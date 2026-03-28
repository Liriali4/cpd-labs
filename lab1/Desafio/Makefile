CC = gcc
CFLAGS = -Wall -pthread
TARGET = cpd-terminal

all: $(TARGET)

$(TARGET): main.o commandlinereader.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o commandlinereader.o

main.o: main.c commandlinereader.h
	$(CC) $(CFLAGS) -c main.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	$(CC) $(CFLAGS) -c commandlinereader.c

clean:
	rm -f *.o $(TARGET)