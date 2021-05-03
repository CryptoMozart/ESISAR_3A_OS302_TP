#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char const *argv[])
{
	int msg_id;
	struct msg_struct msg;

	if (argc != 4) {
		printf("Usage: %s operande1 {+|-|x|/} operande2\n", argv[0]);  //on remplace * par x car ca nous evite de devoir mettre de ' ' quand on lance le client
		return EXIT_FAILURE;
	}

	/* il faut eviter la division par zero */
	if(argv[2][0] == '/' && atoi(argv[3]) == 0){
		perror("Division par 0");
		return EXIT_FAILURE;
	}
/*on vient mettre toutes les donnes dans le message a envoyer*/
	(msg.donnee).op1 = atoi(argv[1]);
	(msg.donnee).op2 = atoi(argv[3]);
	(msg.donnee).operateur = argv[2][0];
	(msg.donnee).pid = getpid();
	msg.type = 1;



	/* ATTENTION : la file de messages doit avoir ete creee par le serveur. Il
	 * faudrait tester la valeur de retour (msg_id) pour verifier que cette
	 * creation a bien eu lieu. */

	/* On prepare un message de type 1 à envoyer au serveur avec les
	 * informations necessaires */

	key_t cle = ftok("fich",0);  //on cree la cle pour notre file de message

	if((msg_id = msgget(cle, 0) ) == -1){  //on vient utiliser la file de message cree par le serveur grace a la cle
		perror("ERROR msgget");
		return EXIT_FAILURE;
	}
	printf("CLIENT %d: preparation du message contenant l'operation suivante:\
		   	%d %c %d\n", getpid(), atoi(argv[1]), argv[2][0], atoi(argv[3]));


	/* envoi du message */

	if(msgsnd(msg_id, &msg,sizeof(struct data),0) == -1){  //on envoie notre message au serveur
		perror("ERROR msgsnd");
		return EXIT_FAILURE;
	};
	//l'appel est bloquant par defaut

	/* reception de la reponse */
	printf("J'attend...\n");


	if(msgrcv(msg_id,&msg,sizeof(struct data),getpid(), 0) == -1){  //on attend la reponse du serveur de type egal au pid du client
		perror("ERROR msgrcv");
		return EXIT_FAILURE;
	}
	printf("Bien recu !\n");
	printf("CLIENT: resultat recu depuis le serveur %d : %d\n", (msg.donnee).pid, (msg.donnee).op1);
	return EXIT_SUCCESS;
}
