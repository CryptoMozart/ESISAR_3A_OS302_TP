#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "struct.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("1 arguments demandé\n");
        exit(EXIT_FAILURE);
    }

    struct msgbuf msg;


    strcpy(msg.mtext,argv[1]);  //on ecrit le message que l'on veut envoyer au serveur
    msg.mtype = 1;

    int msg_id;
    int msg_retour_id;

    key_t cle;
    if((cle = ftok("fich",0)) == -1){  //creation de la cle pour utiliser la 1ere file de message
        perror("ERROR ftok");
        exit(EXIT_FAILURE);
    }

    if((msg_id = msgget(cle,0)) == -1){  //on vient utiliser la 1ere file de message grace a la premiere cle
        perror("ERROR msgget");
        exit(EXIT_FAILURE);
    }

    key_t cle_retour;
    if((cle_retour = ftok("fich2",0)) == -1){  //creation de la cle pour utiliser la 2eme file de message
        perror("ERROR ftok retour");
        exit(EXIT_FAILURE);
    }

    if((msg_retour_id = msgget(cle_retour,0)) == -1){   //on vient utiliser la 2eme file de message grace a la premiere cle
        perror("ERROR msgget retour");
        exit(EXIT_FAILURE);
    }

    msg.pid = getpid();  //on entre notre pid dans le message envoyer au serveur, pour que celui ci puisse nous repondre avec un type egal a ce dernier

    printf("File de message client créée\n");

    /* Envoi du message */

    if(msgsnd(msg_id,&msg,sizeof(struct msgbuf)-sizeof(long),0) == -1){  //on envoie notre message de type 1 avec la 1ere file de message au serveur
        perror("ERROR msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("CLIENT %d: preparation du message\n",msg.pid);

    if(!strcmp(argv[1],"@")){  //si on a demande a arreter le serveur, on a pas besoin d'attednre de reponse et donc le client s'arrete
      printf("Message d'arret du serveur\n");
      return EXIT_SUCCESS;
    }

    printf("En attente d'une réponse: \n");

    if(msgrcv(msg_retour_id,&msg,sizeof(struct msgbuf)-sizeof(long),getpid(),0) == -1){  //on attend la reponse du serveur de type egal au pid du client par la 2eme file de message
		perror("ERROR msgrcv retour");
		exit(EXIT_FAILURE);
	}
    printf("Bien recu !\n");
    printf("Voici la réponse de %d: %s\n",msg.pid,msg.mtext);

    return EXIT_SUCCESS;
}
