#include "scheduler-utils.h"
#include "../ipc/MsgQueue.h"
#include "../ipc/MsgStruct.h"
#include "../DataStructures/Queue.h"



struct PCB pcbArray[MAX_PROCESSES];
int pcbArraySize = 0;
struct Queue* readyQueue;

bool isFinishedGenerating;
bool isProcessRunning = false;
int msgQueueId;

void attachSignalHandlers()
{
    // Attach signal handler to handle a process stop
    signal(SIGCHLD, processStopped);
    // Attach signal handler to recieve processes from process generator
    signal(SIGUSR1, processRecieved);
    signal(SIGUSR2, finishedGeneratingProcess);

}

void processRecieved(int signum) {
    recieveProcess();
}

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

    // TODO: Use the priority ready queue instead of normal queue (and with pcb)
    enqueue(readyQueue, pcbArray[pcbArraySize++].processData); 

}

int runProcess(int runningTime)
{
    printf("Running a process with remaining time %d\n", runningTime);
    int pid = fork();
    if (pid == 0)
    {
        // Child process
        char remainingTimeStr[10];
        sprintf(remainingTimeStr, "%d", runningTime);
        char* args[] = {"./build/process.out", remainingTimeStr, NULL};
        execvp(args[0], args);
    }
    isProcessRunning = true;
    return pid;

}

int runNextProcess()
{
    if (isEmpty(readyQueue)) return -1;
    int pid = runProcess(peak(readyQueue)->pData.runningTime);
    int pcbIndex = getPCBIndex(pid);
    pcbArray[pcbIndex].actualPid = pid;
    pcbArray[pcbIndex].state = RUNNING;
    return pid;

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

void processStopped(int signum)
{
    printf("A process has stopped\n");
    int remainingTime;
    int pid = wait(&remainingTime);
    int pcbIndex = getPCBIndex(pid);
    // TODO: Update the process state
    // TODO: Update the process statistics
    // TODO: Log the process termination and statistics
    // TODO: Delete the process from the pcb array if it has no remaining time

    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)
    dequeue(readyQueue);

    isProcessRunning = false;
}

void finishedGeneratingProcess(int signum)
{
    isFinishedGenerating = true;
}