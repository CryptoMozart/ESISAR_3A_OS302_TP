#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 6
#define TAILLE_MAX 20

int lire_valeur(const char *path);

void wake_up(int sig){}  //fonction qui ne fait rien pour pouvoir juste réveiller un processus mis en pause
void stop(int sig);
void fin(int sig);


int main (int argc, char *argv[]){

  if (argc != 2) {
		printf("Utilisation : %s nbre-de-processus !\n", argv[0]);
		return EXIT_FAILURE;
	}


  signal(SIGUSR1,wake_up);
  signal(SIGUSR2,wake_up);	//pour ne rien faire à la réception de SIGUSR2, car le père va se ping lui meme avec se signal avec un kill(0,SIGUSR2)
  signal(SIGCHLD,fin); //quand le fils est mort (signal SIGCHILD) on exéctue la fonction fin qui va envoyé le signal de "libération" aux autres fils


  pid_t fils[N]; //on créer un tableau de N processus fils 

  for (int i = 1 ; i <= N ; i++) {

		fils[i]= fork();

    if (fils[i] == 0){ // on est dans le ième fils
        printf("%d est créé\n",getpid());
        
        if (fils[i] == -1){ //cas erreur dans la création du fils
        perror("Le fork n'a pas reussi");
        return EXIT_FAILURE;
      }
      signal(SIGUSR1,wake_up);
      signal(SIGUSR2,stop);   //on exécute stop quand on recoit SIGUSR2, donc quand le pere nous "libère"

      while(1){
        pause(); //on attend d'être réveillé par wake_up, donc par le père qui nous envoie un SIGUSR1
       
        int barillet = lire_valeur(argv[1]); //on lit la valeur du barillet dans le fichier passé en argument
       
        if (barillet == i){ //si le numéro est celui du fils il se tue.
          kill(getpid(),SIGKILL); //on lui envoie le signal SIGKILL pour qu'il se tue.
        }
        else{ //on affiche les fils qui sont toujours vivants
          printf("%d est toujours vivant ! \n",i);
          pid_t pere = getppid();
          kill(pere,SIGUSR1);
        }
      }

    }
  }
  sleep(1);  //pour etre sur que tous les fils sont créés
  for (int i = 1 ; i <= N ; i++){
    kill(fils[i],SIGUSR1); // pour chaque fils on va le réveiller avec le signal SIGUSR1
    pause(); //on attend d'être réveillé par wake_up, donc soit que le fils nous dise qu'il est toujours vivant, soit qu'un fils est mort
  }
  return 0;
}


void stop(int sig){
  pid_t pid = getpid();
  printf("Bisous, j'étais %d\n",pid);
  exit(0);
}

void fin(int sig){
  kill(0,SIGUSR2);
  sleep(1);  //pour être sur que tous les fils se sont bien arrêtés avant d'arrêter le père
  exit(0);
}

int lire_valeur(const char *path){
  FILE *fichier;
  char chaine[TAILLE_MAX];
  int valeur;
  fichier = fopen(path, "r");
  if (fichier != NULL) {
   fgets(chaine, TAILLE_MAX, fichier);
   fclose(fichier);
   valeur = atoi(chaine);
  }
  return valeur;
}
