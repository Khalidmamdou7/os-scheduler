#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"


int main(int argc, char *argv[])
{

    printf("SJF scheduling algorithm\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("SJF connected to the message queue with id %d\n", msgQueueId);

    readyQueue = createQueue();
    attachSignalHandlers();

    while (!isFinishedGenerating || isProcessRunning || !isEmpty(readyQueue))
    {
        if (!isProcessRunning)
        {
            if (!isEmpty(readyQueue))
            {
                printf("Scheduler-sjf is running the next process %d\n", peak(readyQueue)->pData.id);
                runNextProcess();
            }
        }
        printf("SJF is sleeping\n");
        sleep(1);
    }


    printf("SJF is done\n");
}