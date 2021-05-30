#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
    unlink("/tmp/fifo"); //on supprime le tube au cas où il serait déja créé

    if(mkfifo("/tmp/fifo",0666) == -1){
        exit(0);
    }
    else{
        char buf[50];
        while(strcmp(buf,"FIN") != 0){

            FILE* file = fopen("/tmp/fifo","r");
            if(file == NULL){
                exit(1);
            }
            else{
                fgets(buf,20,file);
                printf("%s\n",buf); 
            }

        }

    }
    return 0;
}
