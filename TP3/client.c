#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

    FILE* file = fopen("/tmp/fifo","w");
    if(file == NULL){
        exit(0);
    }
    else{
        open("/tmp/fifo",1);
        fputs(argv[1],file);
        fclose(file);
    }

    return 0;
}
