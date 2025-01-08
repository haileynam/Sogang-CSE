CC = gcc
target = zoo
objects = main.o blackcow.o dog.o turtle.o

$(target) : $(objects)
	$(CC) -o $(target) $(objects)

$(objects) : animal.h

.PHONY : clean
clean: 
	rm $(objects)