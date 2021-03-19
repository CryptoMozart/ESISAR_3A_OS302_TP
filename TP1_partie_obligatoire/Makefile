liste_Obj = sync.o fexec.o exec.o nprocess.o users.o

all: $(liste_Obj)
	gcc -Wall sync.o -o sync
	gcc -Wall fexec.o -o fexec
	gcc -Wall exec.o -o exec
	gcc -Wall nprocess.o -o nprocess
	gcc -Wall users.o -o users

sync.o:
	gcc -Wall sync.c -c
fexec.o:
	gcc -Wall fexec.c -c
exec.o: 
	gcc -Wall exec.c -c
nprocess.o:
	gcc -Wall nprocess.c -c
users.o:
	gcc -Wall users.c -c

clean: 
	rm -rf *.o
