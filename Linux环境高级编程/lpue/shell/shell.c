/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description:  shell command
 *
 *        Version:  1.0
 *        Created:  09/06/11 10:47:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#define MAXLINE 1024

void sig_int(int signo)
{
    printf("catch a ctrl-c\n");
    fflush(stdout);

}

void err_sys(char *string)
{
    printf("%s\n");
    exit(1);
}


int main(void) {

    char    buf[MAXLINE];    
    pid_t   pid;   
    int     status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
                err_sys("signal error.");

    printf("%%");

    while(fgets(buf,MAXLINE,stdin)!=NULL)    {
    
        buf[strlen(buf)-1] = '\0';
    
        if((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid == 0)        {
            execlp(buf,buf,(char *) 0 );
            printf("couldn't execute: %s",buf);
            exit(1);
        }
    
        if (( pid == waitpid(pid,&status, 0)) < 0)
            err_sys("waitpid error.");
    
        printf("%%");
    }
}

