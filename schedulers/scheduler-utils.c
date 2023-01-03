#include "scheduler-utils.h"
#include "../ipc/MsgQueue.h"
#include "../ipc/MsgStruct.h"
#include "../memory/memory.h"




struct PCB pcbArray[MAX_PROCESSES];
int pcbArraySize = 0;

bool isFinishedGenerating;
bool isProcessRunning = false;
int msgQueueId;

struct Tree* tree;
int begin, end;


void recieveProcess()
{
    printf("Recieving a process from process generator using msg queue with id %d\n", msgQueueId);

    struct MsgStruct msg = receiveMsg(msgQueueId, 1);
    printf("Received message from process generator: %d %d %d %d\n", msg.data.id, msg.data.arrivalTime, msg.data.runningTime, msg.data.priority);    

    if (pcbArraySize == MAX_PROCESSES)
    {
        printf("Error: Reached maximum number of processes\n");
        return;
    }

    pcbArray[pcbArraySize] = initializePCB(msg.data);

}

int runProcess(int runningTime)
{
    printf("Running a process with remaining time %d\n", runningTime);
    int actualPid = fork();
    if (actualPid == 0)
    {
        // Child process
        char remainingTimeStr[10];
        sprintf(remainingTimeStr, "%d", runningTime);
        char* args[] = {"./build/process.out", remainingTimeStr, NULL};
        execvp(args[0], args);
    }
    isProcessRunning = true;
    return actualPid;

}

int runNextProcess(struct ProcessData* pData)
{
    int pcbIndex = getPCBIndex(pData->id);
    int actualPid = runProcess(pcbArray[pcbIndex].remainingTime);
    pcbArray[pcbIndex].actualPid = actualPid;
    pcbArray[pcbIndex].state = RUNNING;
    return actualPid;

}

int getPCBIndex(int pid)
{
    int i;
    for (i = 0; i < pcbArraySize; i++)
    {
        if (pcbArray[i].pid == pid)
        {
            return i;
        }
    }
    return -1;
}

int getPCBIndexByActualPid(int actualPid)
{
    int i;
    for (i = 0; i < pcbArraySize; i++)
    {
        if (pcbArray[i].actualPid == actualPid)
        {
            return i;
        }
    }
    return -1;
}

void finishedGeneratingProcess(int signum)
{
    isFinishedGenerating = true;
}

bool memAllocate(struct ProcessData process, int* begin, int* end) {
    int done = 0;
    printf("i am in scheduler utils in memAllocate\n");
    printf("tree->root = %d", tree->root->size);
    bool isAllocated = memory_allocation(process, tree->root, &done, begin, end);
    return isAllocated;
}

bool memDeallocate(struct ProcessData process, int* begin, int* end) {
    int done = 0;
    // bool isDeallocated = memory_deallocation(process, tree->root, &done, begin, end);
    // return isDeallocated;
}