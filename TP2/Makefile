all: ex1.o ex2.o ex3.o
	gcc -Wall ex1.o -o ex1
	gcc -Wall ex2.o -o ex2
	gcc -Wall ex3.o -o ex3

ex1.o: 
	gcc -Wall ex1.c -c
ex2.o:
	gcc -Wall ex2.c -c 
ex3.o:
	gcc -Wall ex3.c -c
clean: 
	rm -rf *.o
