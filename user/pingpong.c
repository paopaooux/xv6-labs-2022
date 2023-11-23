#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
char buf[512];
int p[2];
int main(int argc,char *argv[]){
    if(pipe(p)==-1){
        fprintf(2, "Usage: pipe error...\n");
        exit(1);
    }
    
    int pid=fork();
    if(pid==-1){
        fprintf(2, "Usage: fork error...\n");
        exit(1);
    }

    if(pid==0){
        read(p[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
        close(p[0]);

        write(p[1],"pong",4);
        close(p[1]);
    }else{
        write(p[1],"ping",4);
        close(p[1]);
        wait(0);
        
        read(p[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
        close(p[0]);
    }
    exit(0);
}