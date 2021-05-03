all: client.o serveur.o  client2.o serveur2.o
	gcc -Wall client.o -o client
	gcc -Wall serveur.o -o serveur
	gcc -Wall serveur2.o -o serveur2
	gcc -Wall client2.o -o client2
client.o: client.c calcul.h
	gcc -Wall -c client.c
serveur.o: serveur.c calcul.h
	gcc -Wall -c serveur.c
client2.o: client2.c struct.h
	gcc -Wall -c client2.c
serveur2.o: serveur2.c struct.h
	gcc -Wall -c serveur2.c
clean: 
	rm -rf *.o