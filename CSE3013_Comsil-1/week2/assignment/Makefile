cc = gcc
target = main

objects = main.o calc.o solve.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)
main.o: main.c
	$(cc) -c main.c calc.h solve.h
solve.o: solve.c calc.h
	$(cc) -c solve.c calc.h
calc.o: calc.c
	$(cc) -c calc.c

clean:
	rm $(objects) $(target)