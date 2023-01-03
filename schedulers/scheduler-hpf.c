#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/PriorityQueue.h"
#include "../utils/Logger.h"
#include "../DataStructures/Tree.h"

struct PriorQueue* readyQueue;
int runningProcessPcbIndex = -1;

float cpuUtilization = 0;
float avgWeightedTurnaroundTime = 0;
float avgTurnaroundTime = 0;
float avgWaitingTime = 0;

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
                printf("Scheduler-hpf is running the next process %d\n", peek(readyQueue)->pData.id);
                runNextProcess(&peek(readyQueue)->pData);
                runningProcessPcbIndex = getPCBIndex(peek(readyQueue)->pData.id);
                enum LogState state = STARTED;
                if (pcbArray[runningProcessPcbIndex].remainingTime != pcbArray[runningProcessPcbIndex].processData.runningTime)
                    state = RESUMED;
                
                // TODO: Update the process state
                pcbArray[runningProcessPcbIndex].state = RUNNING;
                if (state == STARTED){
                    pcbArray[runningProcessPcbIndex].startTime = getClk();
                    pcbArray[runningProcessPcbIndex].waitingTime = getClk() - pcbArray[runningProcessPcbIndex].processData.arrivalTime;
                } else {
                    pcbArray[runningProcessPcbIndex].waitingTime += getClk() - pcbArray[runningProcessPcbIndex].lastStoppedTime;
                }

                logState(getClk(), pcbArray[runningProcessPcbIndex].processData.id , state,
                        pcbArray[runningProcessPcbIndex].processData.arrivalTime,
                        pcbArray[runningProcessPcbIndex].processData.runningTime,
                        pcbArray[runningProcessPcbIndex].remainingTime,
                        pcbArray[runningProcessPcbIndex].waitingTime);
            }
        }
        printf("HPF is sleeping\n");
        sleep(1);
    }


    avgTurnaroundTime /= pcbArraySize;
    avgWeightedTurnaroundTime /= pcbArraySize;
    avgWaitingTime /= pcbArraySize;
    cpuUtilization /= getClk();
    cpuUtilization *= 100;
    logPerformance(cpuUtilization, avgWeightedTurnaroundTime, avgTurnaroundTime, avgWaitingTime);

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
    while(!isMsgQueueEmpty(msgQueueId))
    {

        recieveProcess();
        
        // TODO: Use the priority ready queue instead of normal queue (and with pcb)
        printf("SJF is allocating memory for process %d\n", pcbArray[pcbArraySize].processData.id);
         bool isAllocated = memAllocate(pcbArray[pcbArraySize].processData, &begin, &end);
        if (isAllocated) {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, ALLOCATED, begin, end);
            Priorenqueue(readyQueue, pcbArray[pcbArraySize].processData, pcbArray[pcbArraySize].processData.priority);
        } else {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, FAILED, begin, end);
            return;
        }

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
        pcbArray[pcbIndex].lastStoppedTime = getClk();

        logState(getClk(), pcbArray[pcbIndex].processData.id, STOPPED,
                pcbArray[pcbIndex].processData.arrivalTime,
                pcbArray[pcbIndex].processData.runningTime,
                pcbArray[pcbIndex].remainingTime,
                pcbArray[pcbIndex].waitingTime);
    }
    else
    {
        pcbArray[pcbIndex].state = TERMINATED;
        pcbArray[pcbIndex].remainingTime = 0;
        pcbArray[pcbIndex].actualPid = -1;
        pcbArray[pcbIndex].finishTime = getClk();
        pcbArray[pcbIndex].turnaroundTime = getClk() - pcbArray[pcbIndex].processData.arrivalTime;
        pcbArray[pcbIndex].weightedTurnaroundTime = (float)pcbArray[pcbIndex].turnaroundTime / pcbArray[pcbIndex].processData.runningTime;

        avgTurnaroundTime += pcbArray[pcbIndex].turnaroundTime;
        avgWeightedTurnaroundTime += pcbArray[pcbIndex].weightedTurnaroundTime;
        avgWaitingTime += pcbArray[pcbIndex].waitingTime;
        cpuUtilization += pcbArray[pcbIndex].processData.runningTime;

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
        
        Priordequeue(readyQueue);
        PriorprintQueue(readyQueue);

    }

    isProcessRunning = false;
}