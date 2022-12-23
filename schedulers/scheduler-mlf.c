#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"


int main(int argc, char *argv[])
{

    printf("MLF scheduling algorithm starting...\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("MLF connected to the message queue with id %d\n", msgQueueId);


    // TODO: Implement MLF algorithm using the helper functions in scheduler-utils.h



    printf("MLF is done\n");

}

