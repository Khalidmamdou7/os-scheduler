#include "scheduler-utils.h"
#include "../ipc/MsgQueue.h"
#include "../ipc/MsgStruct.h"



struct PCB pcbArray[MAX_PROCESSES];
int pcbArraySize = 0;

bool isFinishedGenerating;
bool isProcessRunning = false;
int msgQueueId;


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

//get suitable size for memory allocation using buddy system
int getSuitableSize(int process_size)
{
    int size = 1;
    while (size < process_size)
    {
        size *= 2;
    }
    return size;
}

int allocateMem(int process_size,int process_id)
{
    int suitable_size = getSuitableSize(process_size);
    bool allocated = false;
    //assumption : first fit memory allocated
    for (int i = 0; i < 1024 ; i += suitable_size)//1024 is the size of the memory
    {

    //i= 0,32,64,96... if process_size = 32
    //we need to check if the memory is allocated or not

        allocated = false;

        for (int j = i; j < i+suitable_size; j++)
        {
            if (memory[j] != 0)//allocated
            {
                allocated = true;
                break;
            }
        }
        if (!allocated)
        {
            for (int k = i; k < i+suitable_size; k++)
            {
                memory[k] = process_id;
            }
            //allocated successfully
            //return the starting address of the allocated memory
            return i;
        }
    }
    return -1;//failed to allocate memory for the process
}

int deallocateMem(int process_size,int process_id,int starting_address)
{
    //id not needed in parameters
    int suitable_size = getSuitableSize(process_size);
    for (int i = starting_address; i < starting_address+suitable_size; i++)
    {
        memory[i] = 0;
    }
    return 0;
}