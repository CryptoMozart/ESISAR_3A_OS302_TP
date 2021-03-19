#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_S 60
#define MAX_M 60
#define MAX_H 24


void H(int sig){ //fonction pour gérer les heures
  static int heure = 0;
  heure++;
  printf("\t\t\t\t%d heure(s)\n",heure);
  if( heure == MAX_H)heure = 0;
}

void M(int sig){ //fonction pour gérer les minutes
  static int minute = 0;
  pid_t pere = getppid();
  minute++;
  if (minute==MAX_M){ //quand on atteint 60 minutes, on envoie le signal SIGUSR1 à son père pour compter les heures
    kill(pere,SIGUSR1);
    minute=0;
  }
  else{
  printf("\t\t%d minute(s)\n",minute);
  }
}

void S(int sig){ //fonction pour gérer les secondes
  static int seconde = 1;
  printf("%d Seconde(s)\n",seconde);
  seconde++;

  pid_t pere = getppid();

  if (seconde==MAX_S){ //quand on a atteint 60s, on envoie le signal SIGUSR1 à son père pour compter les minutes.
    kill(pere,SIGUSR1);
    seconde=0;
  }
}

int main(){

  signal(SIGUSR1,H); //on exécute la fonction H à la réception du SIGUSR1 pour le processus père
  pid_t fils_minute = fork();

  if (fils_minute == 0){ //si on est dans fils_minute
    
    signal(SIGUSR1,M); //on exécute la fonction M à la réception du signal SIGUSR1 pour le processus fils_minute
    pid_t fils_seconde =fork();

    if(fils_seconde ==0){  //si on est dans fils_seconde
      signal(SIGALRM,S);   //on exécute la fonction S à la réception du signal SIGALRM dans le processus fils_seconde 
      while(1){
        alarm(1); //la fonction alarm va envoyer un signal SIGALRM après une seconde
        pause(); //on attend de recevoir le prochain SIGALRM de la fonction alarm.
      }

    }
    else if (fils_seconde == -1){ //cas d'erreur
      perror("Le fork n'a pas reussi");
      return EXIT_FAILURE;
    }
    else{  //minute
      while(1){
        pause();
      }
    }
  }
  else if (fils_minute == -1){ //cas d'erreur
    perror("Le fork n'a pas reussi");
    return EXIT_FAILURE;
  }
  else{  //heure
    while(1){
      pause();
    }
  }
  return 0;
}

