#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int msg_id;

void raz_msg(int signal) {  //suppression de la file de messages des la reception d'un signal
	printf("Suppression de la file de message!\n");
	msgctl(msg_id,IPC_RMID,NULL);
}


int main(int argc, char const *argv[])
{
	struct msg_struct msg;
	int i_sig;
	int result;

	/* liberer la zone de messages sur reception de n'importe quel signal */

	for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {  //pour supprimer la file de messages des la reception d'un signal
		signal(i_sig,raz_msg);
	}
	key_t cle = ftok("fich",0);  //creation de la cle pour notre file de message

	if((msg_id = msgget(cle, IPC_CREAT | 0666 ) ) == -1){  //creation de la file de messages grace a la cle
		perror("ERROR msgget");
		return EXIT_FAILURE;
	}

	printf("SERVEUR: pret!\n");

	while (1 == 1) {
		/* reception */
		if( msgrcv(msg_id, &msg, sizeof(struct data), 1, 0) == -1){  //on attend que le client nous envoie un message de type 1 par la file de messages
			perror("ERROR msgrcv");
			return EXIT_FAILURE;
		}
		printf("PID client: %d\nPID serveur: %d\n",msg.donnee.pid,getpid());
		printf("SERVEUR: reception d'une requete de la part de: %d\n",msg.donnee.pid);
		/* preparation de la reponse */
		printf("Calcul...\n");

		printf("%d %c %d \n",(msg.donnee).op1, (msg.donnee).operateur, (msg.donnee).op2);

		msg.type = (msg.donnee).pid;  //type du message de reponse = pid du client
		(msg.donnee).pid = getpid();

		/*on effectue l'operation adequate*/
		switch ((msg.donnee).operateur)
		{
		case '+':
			result = (msg.donnee).op1 + (msg.donnee).op2;
			break;

		case '-':
			result = (msg.donnee).op1 - (msg.donnee).op2;
			break;
		case 'x':
			result = (msg.donnee).op1 * (msg.donnee).op2;
			break;
		case '/':
			result = (msg.donnee).op1 / (msg.donnee).op2;
			break;
		}
		msg.donnee.op1 = result;
		printf("result: %d\n",result);
		/* envoi de la reponse */

		if(msgsnd(msg_id, &msg,sizeof(struct data),0) == -1){  //on nvoie la reponse au client par la file de messages
			perror("ERROR msgsnd");
			return EXIT_FAILURE;
		}
		printf("Réponse bien envoyé\n");
	}
	return EXIT_SUCCESS;
}
