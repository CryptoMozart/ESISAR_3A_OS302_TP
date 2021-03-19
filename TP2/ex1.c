#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//Fonction handler qui s'execute à la reception du signal SUGUSR1.
void handler(int sig){}

int main (){
  signal(SIGUSR1,handler); //on exécute la fonction handler à la réception du SIGUSR1
  pid_t fils = fork();
  if (fils == 0){ //si on est dans le fils
    
    pid_t pere = getppid();
    for(int i=2;i<=100;i+=2){  //le fils affiche les nombres paires
      
      pause();  //le processus se met en pause, il attend d'être réveillé par un signal
      
      printf("%d\n",i);

      kill(pere,SIGUSR1); //le fils envoie à son tour le signal SIGUSR1 au père pour le réveiller
    }
  }
  else if (fils == -1){ //cas d'erreur
    perror("Le fork n'a pas reussi");
    return EXIT_FAILURE;
  }
  else{ //si on est dans le père

    for(int i=1;i<=100;i+=2){ //le père affiche les nombres impaires
      
      sleep(1); //on attend 1s pour être sur que le fils soit en pause.

      printf("%d\n",i);

      kill(fils,SIGUSR1); //on envoie le signal SIGUSR1 au processus fils.

      pause(); //on attend de recevoir un signal du fils pour afficher à nouveau.
    }
  }
  return EXIT_SUCCESS;

}
