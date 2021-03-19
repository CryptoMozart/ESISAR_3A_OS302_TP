#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


void fils(int i);
void pere();
/*
 * Dans main, on indique comment  utiliser les parametres passes sur la
 * ligne de commande
 */
int main(int argc, char *argv[])
{
    int nb_proc;
    int i_fils;
    pid_t pid_fils;

    if (argc != 2) {
        printf("Utilisation : %s nbre-de-processus !\n", argv[0]);
        return EXIT_FAILURE;
    }
nb_proc = atoi(argv[1]); /* Retourne 0 si argv[1] n'est pas un nombre */

    /* Creation des processus fils */
    for (i_fils = 1 ; i_fils <= nb_proc ; i_fils++) {
        pid_fils = fork();
        switch (pid_fils) {
        case 0:
            fils(i_fils);       /* il faut ecrire la fonction fils ... */
            break;
        case -1 :
            perror("Le fork n'a pas reussi");
            return EXIT_FAILURE;
        }
    }
/*
    *  Dans la fonction pere, on utilisera le fait que wait renvoie la valeur
    *  -1 quand il n'y a plus de processus fils a attendre.
    */
    pere();

    return EXIT_SUCCESS;
}

void fils(int i){
    printf("Je suis le fils %d, mon père est %d\n",getpid(),getppid());
    sleep(2*i);
    exit(i);
}

void pere(){
    int etat;
    pid_t ret_wait;

/*on va faire une boucle do while pour pouvoir attendre et recuperer l'etat et le pid du fils qui s'arrete jusqu'a ce que tous les fils s'arretent*/    
    do{
        ret_wait=wait(&etat);
        if(ret_wait != -1) printf ("Le fils : %d c'est termine et son etat etait : %04x\n",ret_wait,etat);
    }while (ret_wait != -1);

    printf("C'est la fin !\n");
}

