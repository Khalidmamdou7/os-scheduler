#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/PriorityQueue.h"

struct PriorQueue* readyQueue;
int runningProcessPcbIndex = -1;

void attachSignalHandlers();
void processStopped(int signum);
void processRecieved(int signum);



int main(int argc, char *argv[])
{

    printf("HPF scheduling algorithm starting...\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("HPF connected to the message queue with id %d\n", msgQueueId);


    // TODO: Implement HPF algorithm using the helper functions in scheduler-utils.h
    readyQueue = createPriorQueue();
    attachSignalHandlers();

    while (!isFinishedGenerating || isProcessRunning || !PriorisEmpty(readyQueue))
    {
        if (!isProcessRunning)
        {
            if (!PriorisEmpty(readyQueue))
            {
                printf("Scheduler-hpf is running the next process %d\n", peek(readyQueue)->pData.id);
                runNextProcess(&peek(readyQueue)->pData);
                runningProcessPcbIndex = getPCBIndex(peek(readyQueue)->pData.id);
                Priordequeue(readyQueue);
            }
        }
        printf("HPF is sleeping\n");
        sleep(1);
    }




    printf("HPF is done\n");

}

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
    
    // TODO: Use the priority ready queue instead of normal queue (and with pcb)
    Priorenqueue(readyQueue, pcbArray[pcbArraySize].processData, pcbArray[pcbArraySize].processData.priority);
    if (peek(readyQueue)->pData.id == pcbArray[pcbArraySize].processData.id)
    {
        if (isProcessRunning)
        {
            printf("Scheduler-hpf is preempting the current running process whic has actual pid %d and pcb index %d\n", 
            pcbArray[runningProcessPcbIndex].actualPid, runningProcessPcbIndex);
            kill(pcbArray[runningProcessPcbIndex].actualPid, SIGUSR1);
        }
    }
    pcbArraySize++;
    PriorprintQueue(readyQueue);

}

void processStopped(int signum)
{
    printf("A process has stopped\n");
    int status;
    int actualPid = wait(&status);
    int remainingTime = status >> 8;
    int pcbIndex = getPCBIndexByActualPid(actualPid);
    // TODO: Update the process state

    // TODO: Update the process statistics
    // TODO: Log the process termination and statistics
    // TODO: Delete the process from the pcb array if it has no remaining time

    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)
    if (remainingTime > 0)
    {
        pcbArray[pcbIndex].remainingTime = remainingTime;
        pcbArray[pcbIndex].state = READY;
        Priorenqueue(readyQueue, pcbArray[pcbIndex].processData, pcbArray[pcbIndex].processData.priority);
    }
    else
    {
        pcbArray[pcbIndex].state = TERMINATED;
        pcbArray[pcbIndex].remainingTime = 0;
        pcbArray[pcbIndex].actualPid = -1;
    }

    isProcessRunning = false;
}