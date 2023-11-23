#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
char buf[512];
int main(int argc,char *argv[]){
    char *p[MAXARG];
    for(int i=1;i<argc;i++){
        p[i-1]=argv[i];
    }
    p[argc-1]=malloc(512);
    while((gets(p[argc-1],512))){
        if(p[argc-1][0]==0){
            break;
        }
        int pid=fork();
        if(p[argc-1][strlen(p[argc-1])-1]=='\n'){ //该函数会将末尾换行保留，故需去掉换行符。
            p[argc-1][strlen(p[argc-1])-1] = 0;
        }
        if(pid==-1){
            fprintf(2,"xargs: fork error...");
            continue;
        }
        if(pid==0){
            exec(argv[1],p);
        }else{
            wait(0);
        }
    }
    exit(0);
} 