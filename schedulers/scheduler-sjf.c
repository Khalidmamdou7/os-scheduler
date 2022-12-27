#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/PriorityQueue.h"
#include "../utils/Logger.h"

struct PriorQueue* readyQueue;
float cpuUtilization;
float avgWeightedTurnaroundTime;
float avgTurnaroundTime;

void attachSignalHandlers();
void processStopped(int signum);
void processRecieved(int signum);



int main(int argc, char *argv[])
{

    printf("SJF scheduling algorithm\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("SJF connected to the message queue with id %d\n", msgQueueId);

    readyQueue = createPriorQueue();
    attachSignalHandlers();

    initClk();

    while (!isFinishedGenerating || isProcessRunning || !PriorisEmpty(readyQueue))
    {
        if (!isProcessRunning)
        {
            if (!PriorisEmpty(readyQueue))
            {
                printf("Scheduler-sjf is running the next process %d\n", peek(readyQueue)->pData.id);
                runNextProcess(&peek(readyQueue)->pData);
                int pcbIndex = getPCBIndex(peek(readyQueue)->pData.id);
                logState(getClk(), pcbArray[pcbIndex].processData.id , STARTED,
                        pcbArray[pcbIndex].processData.arrivalTime,
                        pcbArray[pcbIndex].processData.runningTime,
                        pcbArray[pcbIndex].remainingTime,
                        pcbArray[pcbIndex].waitingTime);
                Priordequeue(readyQueue);
            }
        }
        printf("SJF is sleeping\n");
        sleep(1);
    }

    logPerformance(cpuUtilization, avgWeightedTurnaroundTime, avgTurnaroundTime);
    printf("SJF is done\n");
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
    Priorenqueue(readyQueue, pcbArray[pcbArraySize].processData, pcbArray[pcbArraySize].remainingTime);
    pcbArraySize++;
    PriorprintQueue(readyQueue);

}

void processStopped(int signum)
{
    printf("A process has stopped\n");
    int remainingTime;
    int actualPid = wait(&remainingTime);
    int pcbIndex = getPCBIndexByActualPid(actualPid);
    // TODO: Update the process state

    // TODO: Update the process statistics
    // TODO: Log the process termination and statistics
    logFinished(getClk(), pcbArray[pcbIndex].processData.id, FINISHED,
                pcbArray[pcbIndex].processData.arrivalTime,
                pcbArray[pcbIndex].processData.runningTime,
                pcbArray[pcbIndex].remainingTime,
                pcbArray[pcbIndex].waitingTime,
                pcbArray[pcbIndex].turnaroundTime,
                pcbArray[pcbIndex].weightedTurnaroundTime);
    // TODO: Delete the process from the pcb array if it has no remaining time

    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)
    if (remainingTime > 0)
    {
        pcbArray[pcbIndex].remainingTime = remainingTime;
        pcbArray[pcbIndex].state = READY;
        Priorenqueue(readyQueue, pcbArray[pcbArraySize++].processData, pcbArray[pcbArraySize++].remainingTime);
    }
    else
    {
        pcbArray[pcbIndex].state = TERMINATED;
        pcbArray[pcbIndex].remainingTime = 0;
        pcbArray[pcbIndex].actualPid = -1;
    }

    isProcessRunning = false;
}