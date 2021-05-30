#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>

//nombre d'ouvriers
#define N 7

//on définie sem_id en variable globale car on devra l'utiliser dans le handle
int sem_id;
//on crée trois structure sembuf pour les opérations P,V et le cas nul
struct sembuf sem_p, sem_v, sem_n;

//fonction s'exécutant à la réception du signal SUGUSR1 pour les fils
void handle(int signal){
	semop(sem_id, &sem_v, 1);
	printf("Un ouvrier sort de l'ascenseur\n");
	exit(0);
}

int main(int argc, char const *argv[]){

	//creation sempahore
	key_t key1 = ftok("fich1",0);

	if ((sem_id = semget (key1, 1, IPC_CREAT | 0666))==-1){
		printf("Erreur création semaphore\n");
		return EXIT_FAILURE;
	}
	//on initialise notre semaphore à 2
	if(semctl(sem_id, 0, SETVAL, 2) == -1){
		printf("Erreur initiamisation semaphore\n");
		return EXIT_FAILURE;
	}

//on initialise les valeurs pour les opérations P et V et le cas nul
	sem_v.sem_num = 0;
	sem_v.sem_op = 1;
	sem_v.sem_flg = 0;

	sem_p.sem_num = 0;
	sem_p.sem_op = -1;
	sem_p.sem_flg = 0;

	sem_n.sem_num = 0;
	sem_n.sem_op = 0;
	sem_n.sem_flg = 0;


	pid_t fils[N];

	//creation segment de mémoire partagée
	key_t key2 = ftok("fich2",2);
	int mem_id;
	if ((mem_id = shmget(key2, 10*sizeof(pid_t), IPC_CREAT|0666)) == -1) {
		printf("Erreur création segment\n");
		return EXIT_FAILURE;
	}

	/*initialisation à 0 pour que les fils sachent dans quel "case" du segment de mémoire partagée écrire*/
	pid_t *ini = malloc(2*sizeof(pid_t));
	ini = (pid_t*)shmat(mem_id, NULL, 0);
	if(ini == (pid_t*)-1){
		printf("Erreur d'attachement avec le pere\n");
		return EXIT_FAILURE;
	}
	ini[0] = 0;
	ini[1] = 0;
	if(shmdt(ini)==-1){
		printf("Erreur de détachement initialisation\n");
	return EXIT_FAILURE;
	}

	//création du segment de mémoire pour stocker les pid
	pid_t *attache = malloc(2*sizeof(pid_t));

//on crée les N fils
	for(int i = 0; i<N;i++){
		fils[i] = fork();
		if (fils[i] == -1){
			printf("Erreur création fils\n");
			return EXIT_FAILURE;
		}
		else if (fils[i] == 0){
			//on est dans le ième fils 
			
			//on execute l'opération p (-1)
			semop(sem_id, &sem_p, 1);
			
			//on affecte la fonction handle à la récéption du signal SIGUSR1
			signal(SIGUSR1,handle);

			//on alloue un espace mémoire aléatoire (argument NULL) pour le segment de mémoire partagée 
			attache = (pid_t*)shmat(mem_id, NULL, 0);
			if(attache == (pid_t*)-1){
				printf("Erreur d'attachement avec le fils\n");
				return EXIT_FAILURE;
			}
			//on stocke les pid dans fils dans le segment de mémoire partagée, dans attache[0] si on est le premier à rentrer dans l'ascenseur ou si on est seul, et dans attache[1] si on est le deuxieme a entrer dans l'ascenseur
			if (attache[0] == 0){
				attache[0]=getpid();
			}
			else{
				attache[1]=getpid();
			}
			if(shmdt(attache)==-1){
				printf("Erreur de détachement avec un des fils\n");
				return EXIT_FAILURE;
			}
			//printf("%d attend \n",getpid());

			//en attente d'un signal pour stoper les fils
			pause();
		}
	}

	//on est dans le père
	int i = N;

	// on associe également dans le père le segment de mémoire partagée
	attache = (pid_t*)shmat(mem_id, NULL, 0);
	if(attache == (pid_t*)-1){
		printf("Erreur d'attachement avec le pere\n");
		return EXIT_FAILURE;
	}

	while (i !=0){
		if (i != 1){ //cas pair 
			semop(sem_id, &sem_n, 1); //processus bloqué tant que le semaphore n'est pas nul
			sleep(1);	//temps que les ouvriers montent dans l'ascenseur
			//on récupère les pids des ouvriers (fils) monté dans l'ascenceur 
			pid_t fils1 = attache[0];
			pid_t fils2 = attache[1];
			printf("%d et %d sont montés dans l'ascenseur\n",fils1,fils2);
			attache[0] = 0;
			attache[1] = 0;
			sleep(1);	//temps que l'ascenseur monte
			printf("%d et %d sont arrivés en haut\n",fils1,fils2);
			//on envoie un signal aux fils pour qu'ils sortent de l'ascensseur
			kill(fils1,SIGUSR1);
			kill(fils2,SIGUSR1);
			sleep(1);	//temps que les ouvriers sortent
			i-=2;
			sleep(1);	//le temps que l'ascenseur redescende
		}
		else{ //cas impair, si N est impair, à la fin il reste un ouvrier
			if (semctl(sem_id, 0, GETVAL, 0) == 1){	//si on a bien un ouvrier dans l'ascenseur, la valeur du semaphore est 1
				sleep(1);	//temps que l'ascenseur monte
				pid_t fils = attache[0];
				printf("%d est monté dans l'ascenseur\n",fils);
				attache[0] = 0;
				sleep(1);	//temps que l'ascenseur monte
				printf("%d est arrivé en haut\n",fils);
				kill(fils,SIGUSR1);
				sleep(1);	//temps que l'ouvrier sorte
				i--;
				sleep(1);	//le temps que l'ascenseur redescende
			}
		}
	}
	printf("Fermeture segment et sémaphore\n");
	if(shmdt(attache)==-1){
		printf("Erreur de détachement avec le pere\n");
		return EXIT_FAILURE;
	}
	if(semctl(sem_id, 0, IPC_RMID, NULL)==-1){
		printf("Erreur de suppression du semaphore\n");
		return EXIT_FAILURE;
	}
	if(shmctl(mem_id, 0, IPC_RMID)==-1){
	printf("Erreur de suppression du segment\n");
	return EXIT_FAILURE;
	}

	return 0;
}
