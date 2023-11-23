#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char buf[512];
char* get_name(char *path){//获取当前文件名
    char *p;
    for(p=path+strlen(path);p>=path&&*p!='/';p--);
    p++;
    return p;
}
void find(char *path, char *str)
{
    
    struct dirent de; // 目录结构体
    struct stat st;   // 文件结构体
    int fd = open(path, 0);
    if (fd == -1)
    {
        fprintf(2, "find: cannot open %s...\n", path);
        return ;
    }
    if (fstat(fd, &st) == -1)
    {
        fprintf(2, "find: cannot stat %s...\n", path);
        close(fd);
        return ;
    }
    if(st.type==T_DEVICE){//设备文件
    }else if(st.type==T_FILE){//普通文件
        if(!strcmp(str,get_name(path))){
            printf("%s\n",path);
        }
    }else if(st.type==T_DIR){//目录
        strcpy(buf,path);
        char *p=buf+strlen(buf);
        *p='/';
        p++;
        while(read(fd,&de,sizeof(de))==sizeof(de)){
            if(de.inum==0){//该目录为空或未使用
                continue;
            }
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ]=0;
            if(stat(buf,&st)<0){//通过文件路径将对应的文件信息放入文件结构体中，若失败，则返回-1
                fprintf(2, "find: cannot stat %s...\n", buf);
                continue;
            }
            if(st.type==T_DEVICE||st.type==T_FILE){//为非目录文件
                if(!strcmp(str,get_name(buf))){
                    printf("%s\n",buf);
                }
            }else if(st.type == T_DIR && strcmp(".", get_name(buf)) && strcmp("..", get_name(buf))){//判定为子目录，递归处理，注意不要重复进入本目录以及父目录
        	    	    find(buf, str);
            }
        }
    }
    close(fd);
    return ;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "find: find error...\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}