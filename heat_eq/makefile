all :comp clean
comp :bound calc
	mpicc -o term term.c bound.o calc.o -lm
bound :bound.c bound.h
	gcc -c bound.c
calc :calc.c calc.h const.h
	mpicc -c calc.c 
clean :
	rm ./bound.o calc.o
