#include "headers.h"
#include "enums.h"
#include "./ipc/MsgStruct.h"
#include "./ipc/MsgQueue.h"

int main(int argc, char *argv[])
{
    initClk();

    if (argc < 3)
    {
        printf("Error: Scheduling algorithm is not specified\n");
        printf("Run the program as: ./scheduler.out -sch <scheduling_algorithm_number> <scheduling_algorithm_parameters>\n");
        exit(1);
    }
    enum SchedulingAlgorithm schAlg = atoi(argv[2]);

    printf("Scheduler started with %d scheduling algorithm\n", schAlg);

    key_t msgKey = ftok("keyfile", 65);
    int msgQueueId = getMsgQueue(msgKey);

    while (1)
    {
        struct MsgStruct msg = receiveMsg(msgQueueId, 1);
        printf("Received message from process generator: %d %d %d %d\n", msg.data.id, msg.data.arrivalTime, msg.data.runningTime, msg.data.priority);        
    }
    


    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    printf("Scheduler terminated\n");
    // destroyClk(true);
}
