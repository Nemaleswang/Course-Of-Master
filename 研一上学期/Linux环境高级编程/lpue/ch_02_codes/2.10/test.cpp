#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
  printf("real uid: %d, real gid: %d\n",getuid(),getgid());
  printf("effective uid: %d, effective gid: %d\n",geteuid(),getegid());
  
  exit(0);
}
