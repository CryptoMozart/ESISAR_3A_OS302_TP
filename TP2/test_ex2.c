#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define Max_S 60
#define Max_M 60
#define Max_H 24

int cnt_S = 0;
int cnt_M = 0;
int cnt_H = 0;

pid_t Sec;
pid_t Min;

void handler_S(int sig){
    alarm(1);
    cnt_S ++;
    if(cnt_S == Max_S){
        kill(Sec,SIGUSR1);
        cnt_S = 0;
    }
    printf("S: %d\tM: %d\tH: %d\n",cnt_S,cnt_M,cnt_H);
}

void handler_M(int sig){
    cnt_M ++;
    if(cnt_M == Max_M){
        kill(Min,SIGUSR2);
        cnt_M = 0;
    }
}

void handler_H(int sig){
    if(cnt_M == Max_M)cnt_H ++;
    if(cnt_H == Max_H){
        cnt_H = 0;
    }
}

int main(){
    
    Sec = fork();

    signal(SIGUSR1,handler_M);
    signal(SIGALRM, handler_S);
    signal(SIGUSR2,handler_H);


    if(Sec == 0){
        alarm(1);
        while(1);
    }
    else{
        Min = fork();

        if(Min == 0){
            while(1);
        }
    }
    while(1);
    
return 0;
}
    