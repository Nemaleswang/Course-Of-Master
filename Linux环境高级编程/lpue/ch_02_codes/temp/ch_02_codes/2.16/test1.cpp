#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;

int main()
{
  umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  creat("2.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  return 0;
}
