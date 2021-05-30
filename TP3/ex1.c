#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char* argv[]){
    if(argc != 3){
        perror("Donner deux commandes en arguments");
        exit(EXIT_FAILURE);
    }
    else{
        pid_t fils;
        int p[2]; //tableau de 2 int pour le tube
        if(pipe(p) == -1){
            perror("Erreur création tube");
            return EXIT_FAILURE;
        }
        if((fils = fork()) ==  0){
            //on est dans le fils
            dup2(p[1],STDOUT_FILENO); //on redirige l'entrée vers la sorite
            system(argv[1]);
        }
        else{
            //on est dans le père
            close(p[1]); //on ferme l'entrée 
            dup2(p[0],STDIN_FILENO);
            system(argv[2]);
	    close(p[0]); //on ferme la sortie
	    return EXIT_SUCCESS;
        }
    }
}
