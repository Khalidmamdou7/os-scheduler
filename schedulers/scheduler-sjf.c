#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/PriorityQueue.h"
#include "../utils/Logger.h"
#include "../DataStructures/Tree.h"

struct PriorQueue* readyQueue;
float cpuUtilization;
float avgWeightedTurnaroundTime;
float avgTurnaroundTime;
float avgWaitingTime;

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
    tree = createTree();
    printTree(tree->root);
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
                pcbArray[pcbIndex].startTime = getClk();
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

    avgWeightedTurnaroundTime /= pcbArraySize;
    avgTurnaroundTime /= pcbArraySize;
    avgWaitingTime /= pcbArraySize;
    cpuUtilization /= getClk();
    cpuUtilization *= 100;
    logPerformance(cpuUtilization, avgWeightedTurnaroundTime, avgTurnaroundTime, avgWaitingTime);
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
    while(!isMsgQueueEmpty(msgQueueId))
    {

        recieveProcess();
        
        // TODO: Use the priority ready queue instead of normal queue (and with pcb)
        // Allocate memory for the process
        printf("SJF is allocating memory for process %d\n", pcbArray[pcbArraySize].processData.id);
        bool isAllocated = memAllocate(pcbArray[pcbArraySize].processData, &begin, &end);
        if (isAllocated) {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, ALLOCATED, begin, end);
            Priorenqueue(readyQueue, pcbArray[pcbArraySize].processData, pcbArray[pcbArraySize].remainingTime);
        } else {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, FAILED, begin, end);
            return;
        }
        pcbArraySize++;
        PriorprintQueue(readyQueue);
    }

}

void processStopped(int signum)
{
    printf("A process has stopped\n");
    int remainingTime;
    int actualPid = wait(&remainingTime);
    int pcbIndex = getPCBIndexByActualPid(actualPid);
    // TODO: Update the process state
    pcbArray[pcbIndex].state = TERMINATED;
    pcbArray[pcbIndex].remainingTime = 0;
    pcbArray[pcbIndex].actualPid = -1;
    pcbArray[pcbIndex].finishTime = getClk();
    printf("process running time %d\n", pcbArray[pcbIndex].processData.runningTime);
    pcbArray[pcbIndex].waitingTime = pcbArray[pcbIndex].finishTime - pcbArray[pcbIndex].startTime - pcbArray[pcbIndex].processData.runningTime;
    pcbArray[pcbIndex].turnaroundTime = pcbArray[pcbIndex].finishTime - pcbArray[pcbIndex].processData.arrivalTime;
    pcbArray[pcbIndex].weightedTurnaroundTime = (float)pcbArray[pcbIndex].turnaroundTime / pcbArray[pcbIndex].processData.runningTime;

    // TODO: Update the process statistics
    avgTurnaroundTime += pcbArray[pcbIndex].turnaroundTime;
    avgWeightedTurnaroundTime += pcbArray[pcbIndex].weightedTurnaroundTime;
    avgWaitingTime += pcbArray[pcbIndex].waitingTime;
    cpuUtilization += pcbArray[pcbIndex].processData.runningTime;

    // TODO: Log the process termination and statistics
    bool isDeallocated = memDeallocate(pcbArray[pcbIndex].processData, &begin, &end);
    if (isDeallocated)
        logMemory(getClk(), pcbArray[pcbIndex].processData.id, pcbArray[pcbIndex].processData.size, DEALLOCATED, begin, end);
    else
        logMemory(getClk(), pcbArray[pcbIndex].processData.id, pcbArray[pcbIndex].processData.size, FAILED, begin, end);
    logFinished(getClk(), pcbArray[pcbIndex].processData.id, FINISHED,
                pcbArray[pcbIndex].processData.arrivalTime,
                pcbArray[pcbIndex].processData.runningTime,
                pcbArray[pcbIndex].remainingTime,
                pcbArray[pcbIndex].waitingTime,
                pcbArray[pcbIndex].turnaroundTime,
                pcbArray[pcbIndex].weightedTurnaroundTime);
    // TODO: Delete the process from the pcb array if it has no remaining time

    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)

    isProcessRunning = false;
}