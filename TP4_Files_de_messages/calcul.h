#ifndef __CALCUL_H__
#define __CALCUL_H__

#include <unistd.h>
#include <sys/types.h>


struct data {
    int op1;
    int op2;
    char operateur;
    pid_t pid;
};

struct msg_struct {
    long type;
    struct data donnee;
};



#endif /* __CALCUL_H__ */
