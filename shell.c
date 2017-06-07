/*
*
* Author：　　　ShooterIT
* Datatime: 　　2016-9-25 17:31:13
* description：简单shell
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARGS 128

//输出错误并退出
void perror(const char *msg)
{
    fprintf(stderr, "%s:%s\n", msg ,strerror(errno));
    exit(errno);
}

//内置命令
int builtin_command(const char **argv)
{
    if(!strcmp(argv[0] ,"quit") || !strcmp(argv[0] ,"q")){
        exit(0);
    }
    if(!strcmp(argv[0] ,"usage")){
        printf("command [argument].\n");
        return 1;
    }
    return 0;
}

//解析命令
void parseline(char *buf, char **argv)
{
    int argc = 0;
    while (isspace(*buf)) {
        buf++;
    }
    argv[argc] = buf;
    while (*buf != '\0') {
        if(isspace(*buf)){
            *buf = '\0';
            if((*(buf+1)) != '\0' && !isspace(*(buf + 1))){
                argv[++argc] = buf + 1;
            }
        }
        buf++;
    }
}

//执行命令行
void excute(const char *cmdline)
{
    char *argv[MAXARGS];    //参数列表
    char  buf[MAXLINE];
    pid_t pid = -1;

    //解析命令
    strcpy(buf, cmdline);
    parseline(buf, argv);

    //空行
    if(argv[0] == NULL){
        return;
    }

    //处理
    if(!builtin_command(argv)){
        //创建进程
        if((pid = fork()) < 0){
            perror("fork error");
        }else if(pid == 0){
            //子进程
            if(execvp(argv[0], argv) < 0){
                perror("commang error");
            }
            exit(0);
        }else{
            //父进程
            int status;
            if(waitpid(pid, &status, 0) < 0){
                perror("waitpid error");
            }
        }
    }
}

//主函数
int main(int argc, int argv)
{
    char cmdline[MAXLINE];
    while (1) {
        printf("> ");
        fgets(cmdline, MAXLINE, stdin);
        if(feof(stdin)){
            exit(0);
        }
        excute(cmdline);
    }
    return 0;
}
