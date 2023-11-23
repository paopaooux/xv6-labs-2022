#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void get_primes(int p1[]){
    close(p1[1]);
    int n;
    int num=read(p1[0],&n,sizeof(n));
    if(!num){
        close(p1[0]);
        exit(0);
    }
    printf("prime %d\n",n);
    int p2[2];
    if(pipe(p2)==-1){
        fprintf(2, "Usage: pipe error...\n");
        exit(1);
    }
    int pid=fork();
    if(pid<0){
        fprintf(2, "Usage: fork error...\n");
        exit(1);
    }
    if(pid>0){
        int m;
        while(read(p1[0],&m,sizeof(m))){
            if(m%n!=0){
                write(p2[1],&m,sizeof(m));
            }
        }
        close(p1[0]);
        close(p2[1]);
        wait(0);
    }else{
        get_primes(p2);
    }
    exit(0);
}
int main(int argc,char *arcv[]){
    int p[2];
    if(pipe(p)==-1){
        fprintf(2, "Usage: pipe error...\n");
        exit(1);
    }
    for(int i=2;i<=35;i++){
        write(p[1],&i,sizeof(i));
    }
    get_primes(p);
    exit(0);
}