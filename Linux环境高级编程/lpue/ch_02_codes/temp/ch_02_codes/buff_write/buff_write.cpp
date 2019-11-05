#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
     int n,buffsize;
     char *buff;
     if(argc != 2) {
            printf("Usage:%s #n\n",argv[0]);
            exit(1);
     }       

     buffsize = atoi(argv[1]);

     if((buff = (char *)malloc(buffsize)) == NULL)

        printf("malloc memory error.\n");

     while((n = read(STDIN_FILENO,buff,buffsize)) > 0)

         if(write(STDOUT_FILENO,buff,n) != n)

             printf("write error.\n");

     if(n < 0)

         printf("read error.\n");

     exit(0);
}    

