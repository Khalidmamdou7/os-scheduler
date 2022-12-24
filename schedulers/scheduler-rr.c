#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"


int main(int argc, char *argv[])
{

    printf("RR scheduling algorithm starting...\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("RR connected to the message queue with id %d\n", msgQueueId);


    // TODO: Implement RR algorithm using the helper functions in scheduler-utils.h



    printf("RR is done\n");

}

