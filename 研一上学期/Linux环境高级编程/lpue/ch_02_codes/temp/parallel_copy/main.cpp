
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include "global.h"

using std::cout;
using std::cerr;
using std::endl;

int 
main(int argc, char **argv)
{
	pthread_t *tids;
	int cnt = 0;
    int nProducers, nConsumers;
    if (argc != 3)
    {
        cerr << "usage: " << argv[0]
            << " nProducers nConsumers" << endl;
        return -1;
    }

	nProducers = atoi(argv[1]);
	nConsumers = atoi(argv[2]);

	tids = (pthread_t *) malloc((nProducers + nConsumers) * sizeof(pthread_t));

    bufferInit(-1);
    fileFd = open("output.res", O_CREAT | O_APPEND | O_TRUNC | O_WRONLY,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fileFd == -1)
    {
        perror("open file result fialed");
        return -1;
    }

    if (pipe(pipeFd) == -1)
    {
        perror("creat pipe error");
        return -1;
    }

    for (int i = 0; i < nProducers; ++i)
    {
       if(pthread_create(&tids[cnt++], NULL, blkProducer, NULL) == -1)
           perror("create producer failed");
    }

    for (int i = 0; i < nConsumers; ++i)
    {
       if(pthread_create(&tids[cnt++], NULL, blkConsumer, NULL) == -1)
           perror("create consumer failed");
    }

    pthread_mutex_lock(&stdoutMutex);
    cout << "init success!" << endl;
    pthread_mutex_unlock(&stdoutMutex);

	for (int i = 0; i < nProducers; ++i )
	{
		pthread_join(tids[i], NULL);
		pthread_mutex_lock(&stdoutMutex);
		cout << "producers " << i << " exit" << endl;
		pthread_mutex_unlock(&stdoutMutex);
	}


	for (int i = 0; i < nConsumers; ++i )
	{
		pthread_join(tids[nProducers + i], NULL);
		pthread_mutex_lock(&stdoutMutex);
		cout << "consumer " << i << " exit" << endl;
		pthread_mutex_unlock(&stdoutMutex);
	}
    return 0;
}
