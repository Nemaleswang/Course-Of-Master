/*
 * =====================================================================================
 *
 *       Filename:  list.cpp
 *
 *    Description:  list command
 *
 *        Version:  1.0
 *        Created:  09/06/11 10:33:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jackie, 
 *        Company:  uestc
 *
 * =====================================================================================
 */

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
void err_quit(char * string)
{
    printf("%s\n", string);
    exit(1);
}


int main(int argc, char *argv[])
{
   DIR *dp;
   struct dirent       *dirp;
    
   if(argc != 2)
       err_quit("a single argument is required");
   if((dp = opendir(argv[1])) == NULL)
   { 
       printf("can't open %s",argv[1]);
       exit(1);
   }

   while(( dirp = readdir(dp)) != NULL)
       printf("%s\n",dirp->d_name);

   closedir(dp);
                                                                                        exit(0);
                                                                                        }

