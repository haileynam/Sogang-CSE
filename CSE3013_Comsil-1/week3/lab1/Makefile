OBJECTS = main.o array.o rangearray.o
SRC = main.cpp array.cpp rangearray.cpp

CC = g++
TARGET = main
$(TARGET): $(OBJECTS) Array.h RangeArray.h
		$(CC) -o $(TARGET) $(OBJECTS)


.PHONY: clean
clean: 
		rm $(TARGET) $(OBJECTS)