#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/Queue.h"
#include "../utils/Logger.h"

struct Queue* readyQueue;
float cpuUtilization = 0;
float avgWeightedTurnaroundTime = 0;
float avgTurnaroundTime = 0;
float avgWaitingTime = 0;

void processRecieved();
void attachSignalHandlers();
void quantumFinished();

void processStopped(int signum)
{
    printf("A process has stopped\n");
    int remainingTime;
    int actualPid = wait(&remainingTime);
    remainingTime=remainingTime>>8;
    printf("actual pid of process stopped = %d with remaining time=%d \n",actualPid,remainingTime);
    int pcbIndex = getPCBIndexByActualPid(actualPid);
    // TODO: Update the process state
    pcbArray[pcbIndex].remainingTime=remainingTime;
    pcbArray[pcbIndex].lastStoppedTime = getClk();
    pcbArray[pcbIndex].state=remainingTime==0? TERMINATED:READY;
    // TODO: Update the process statistics

    // TODO: Log the process termination and statistics
    if (pcbArray[pcbIndex].state == TERMINATED)
    {
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
    }
    else
        logState(getClk(), pcbArray[pcbIndex].processData.id, STOPPED,
                 pcbArray[pcbIndex].processData.arrivalTime,
                 pcbArray[pcbIndex].processData.runningTime,
                 pcbArray[pcbIndex].remainingTime,
                 pcbArray[pcbIndex].waitingTime);
    // TODO: Delete the process from the pcb array if it has no remaining time

    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)
    dequeue(readyQueue);
    if(pcbArray[pcbIndex].remainingTime>0)
    enqueue(readyQueue,pcbArray[pcbIndex].processData);

    isProcessRunning = false;
}

int main(int argc, char *argv[])
{
    printf("RR scheduling algorithm\n");

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);                                       
    printf("RR connected to the message queue with id %d\n", msgQueueId);
    initClk();

    readyQueue = createQueue();
    attachSignalHandlers();
    int Q=atoi(argv[1]);


    while (!isFinishedGenerating || isProcessRunning || !isEmpty(readyQueue))
    {
        if (!isProcessRunning)
        {
            if (!isEmpty(readyQueue))
            {
                struct QueueNode* p=readyQueue->front;
                printf("Scheduler-RR is running the next process %d\n", p->pData.id);
                
                runNextProcess(&p->pData);
                int pcbindex=getPCBIndex(p->pData.id);
                
                enum LogState state = STARTED;
                if (pcbArray[pcbindex].remainingTime != pcbArray[pcbindex].processData.runningTime)
                {
                    state = RESUMED;
                    pcbArray[pcbindex].waitingTime += getClk() - pcbArray[pcbindex].lastStoppedTime;
                } else {
                    pcbArray[pcbindex].startTime = getClk();
                    pcbArray[pcbindex].waitingTime = getClk() - pcbArray[pcbindex].processData.arrivalTime;
                }
                logState(getClk(), pcbArray[pcbindex].processData.id, state,
                        pcbArray[pcbindex].processData.arrivalTime,
                        pcbArray[pcbindex].processData.runningTime,
                        pcbArray[pcbindex].remainingTime,
                        pcbArray[pcbindex].waitingTime);

                /*sleep(Q);
                printf("stop process with id =%d \n",pcbArray[pcbindex].actualPid);
                kill(pcbArray[pcbindex].actualPid,SIGUSR1);*/
                if(pcbArray[pcbindex].remainingTime>Q)
                {
                    printf("at time %d, alarm has been set for %d \n",getClk(),Q);
                    int timer = getClk();
                    while (getClk() - timer < Q) {};
                    
                }
                else{
                    printf("at time %d, alarm has been set for %d \n",getClk(),pcbArray[pcbindex].remainingTime);
                    int timer = getClk();
                    while (getClk() - timer < pcbArray[pcbindex].remainingTime) {};
                    // sleep(pcbArray[pcbindex].remainingTime);
                }

            }
            while (!isMsgQueueEmpty(msgQueueId))
            {
                processRecieved();
            }
            quantumFinished();
        }
    }

    avgTurnaroundTime /= pcbArraySize;
    avgWeightedTurnaroundTime /= pcbArraySize;
    avgWaitingTime /= pcbArraySize;
    cpuUtilization /= getClk();
    cpuUtilization *= 100;
    logPerformance(cpuUtilization, avgWeightedTurnaroundTime, avgTurnaroundTime, avgWaitingTime);

    printf("RR is done\n");

}
void attachSignalHandlers()
{
    // Attach signal handler to handle a process stop
    signal(SIGCHLD, processStopped);
    // Attach signal handler to recieve processes from process generator
    // signal(SIGUSR1, processRecieved);
    signal(SIGUSR2, finishedGeneratingProcess);
    // signal(SIGALRM,quantumFinished);

}
void processRecieved() {

    while(!isMsgQueueEmpty(msgQueueId))
    {
        recieveProcess();
        
        // TODO: Use the priority ready queue instead of normal queue (and with pcb)
        enqueue(readyQueue, pcbArray[pcbArraySize].processData);
        // Allocate memory for the process
        printf("SJF is allocating memory for process %d\n", pcbArray[pcbArraySize].processData.id);
        bool isAllocated = memAllocate(pcbArray[pcbArraySize].processData, &begin, &end);
        if (isAllocated) {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, ALLOCATED, begin, end);
        } else {
            logMemory(getClk(), pcbArray[pcbArraySize].processData.id, pcbArray[pcbArraySize].processData.size, FAILED, begin, end);
        }
        pcbArraySize++;
        printQueue(readyQueue);

    }


}
void quantumFinished()
{
    printf("At time %d, quantum finished\n",getClk());
    if(isProcessRunning)
    {
        int actualpid=pcbArray[getPCBIndex(peak(readyQueue)->pData.id)].actualPid;
        kill(actualpid,SIGUSR1);
    }

}
