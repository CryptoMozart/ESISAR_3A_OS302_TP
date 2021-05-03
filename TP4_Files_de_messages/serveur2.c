#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "struct.h"

int msg_id;
int msg_retour_id;

void raz_msg(int signal) {  //suppression des 2 files de messages des la reception d'un signal
	printf("Suppression des files de message!\n");
	msgctl(msg_id,IPC_RMID,NULL);
	printf("1 suppr\n");
  msgctl(msg_retour_id,IPC_RMID,NULL);
	printf("2 suppr\n");
}

int main(){

    struct msgbuf msg;

    int i_sig;

    for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {  //pour supprimer les 2 files de messages des la reception d'un signal
			signal(i_sig,raz_msg);
    }

    key_t cle;

    if((cle = ftok("fich",0)) == -1){  //creation de la premiere cle
        perror("ERROR ftok");
        exit(EXIT_FAILURE);
    }

    if((msg_id = msgget(cle,IPC_CREAT | IPC_EXCL | 0666)) == -1){  //creation de la 1ere file a partir de la premiere cle), celle que le client utilisera pour envoyer son message
        perror("ERROR msgget");
        exit(EXIT_FAILURE);
    }

    key_t cle_retour;

    if((cle_retour = ftok("fich2",0)) == -1){  //creation de la deuxieme cle
     perror("ERROR ftok retour");
        exit(EXIT_FAILURE);
    }

    if((msg_retour_id = msgget(cle_retour,IPC_CREAT | IPC_EXCL | 0666)) == -1){  //creation de la 1ere file (a partir de la deuxieme cle), celle que le serveur utilisera pour envoyer sa reponse
        perror("ERROR msgget retour");
        exit(EXIT_FAILURE);
    }


    printf("SERVEUR PRET: \n");


    while (1 == 1)
    {
        /* Reception de la demande du client */

        if(msgrcv(msg_id,&msg,sizeof(struct msgbuf)-sizeof(long),1,0) == -1){  //attente d'un message du client de type 1 par la 1ere file de message
						msgctl(msg_id,IPC_RMID,NULL);
						msgctl(msg_retour_id,IPC_RMID,NULL);
            perror("ERROR msgrcv");
            exit(EXIT_FAILURE);
        }
				if(!strcmp(msg.mtext,"@")){  //on a demande a fermer le serveur
					printf("Fermeture du serveur\n");
					/*suppression des deux files de messages*/
					msgctl(msg_id,IPC_RMID,NULL);
					msgctl(msg_retour_id,IPC_RMID,NULL);

					return EXIT_SUCCESS;
				}
        printf("PID client: %d\nPID serveur: %d\n",msg.pid,getpid());
				printf("SERVEUR: reception d'une requete de la part de: %d\n",msg.pid);

        msg.mtype = msg.pid;  //type du message de reponse = pid du client
        msg.pid = getpid();

        majuscule(msg.mtext);  //on utilise une fonction implemente dans struct.h pour passer en majuscule
        printf("chaine de retour: %s\n",msg.mtext);

        /* Renvoie de la réponse */

        if(msgsnd(msg_retour_id, &msg,sizeof(struct msgbuf)-sizeof(long),0) == -1){  //on envoie la reponse au client par la 2eme file de message
					perror("ERROR msgsnd");
					exit(EXIT_FAILURE);
		}
		printf("Réponse bien envoyé\n");
    }

    return EXIT_SUCCESS;
}
