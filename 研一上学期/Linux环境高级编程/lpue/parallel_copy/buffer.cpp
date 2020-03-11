
#include "global.h"

void
bufferInit(int val)
{
    for (int i = 0; i < bufferSize; ++i)
        buffer[i] = val;
    return;
}
