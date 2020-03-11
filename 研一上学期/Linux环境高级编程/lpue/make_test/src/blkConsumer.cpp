
#include "global.h"

using std::cout;
using std::cerr;
using std::endl;

void *
blkConsumer(void *arg)
{
	char *ptr;
    int blkId, status;
	struct Block blk;


	ptr = (char *)(&blk);

    for (;;)
    {
        pthread_mutex_lock(&bufferMutex);
		while(bufferSlot == bufferSize)
			pthread_cond_wait(&bufferReadCond, &bufferMutex);
        for (int i = 0; i < bufferSize; ++i )
        {
            if (buffer[i] != -1)
            {
                blkId = buffer[i];
                buffer[i] = -1;
                break;
            }
        }
		++bufferSlot;

		pthread_mutex_lock(&stdoutMutex);
		cout << "bufferSlot is: " << bufferSlot << endl
			<< "get one block, block id is: " << blkId
			<< endl;
		pthread_mutex_unlock(&stdoutMutex);

        pthread_cond_signal(&bufferWriteCond);
        pthread_mutex_unlock(&bufferMutex);


		status = 0;
        if (write(fileFd, blockPtrs[blkId], blockSize) == -1)
			status = -1;

		blk.blk_id = blkId;
		blk.blk_stat = status; 


		if (write(pipeFd[1], ptr, 8) == -1)
		{
			pthread_mutex_lock(&stderrMutex);
			perror("write pipe error");
			pthread_mutex_unlock(&stderrMutex);
			return NULL;
		}
    }
    return NULL;
}

