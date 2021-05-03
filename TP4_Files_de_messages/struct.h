#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <unistd.h>
#include <sys/types.h>

#define SIZE 120

struct msgbuf
{
long mtype;
pid_t pid;
char mtext[120];
};

void majuscule(char *chaine)
{
    int i = 0;

    while (chaine[i] != '\0')
    {
        if(chaine[i]  >= 97 &&  chaine[i] <= 122){
            chaine[i] = chaine[i] - 32;
        }
        i++;
    }
}

#endif
