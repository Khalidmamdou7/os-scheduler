#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include "scheduler-utils.h"
#include "../DataStructures/Queue.h"
struct Queue* readyQueue;
void processRecieved(int signum);
void attachSignalHandlers();
void quantumFinished(int signum);

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
    pcbArray[pcbIndex].state=remainingTime==0? TERMINATED:READY;
    // TODO: Update the process statistics

    // TODO: Log the process termination and statistics
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
                /*sleep(Q);
                printf("stop process with id =%d \n",pcbArray[pcbindex].actualPid);
                kill(pcbArray[pcbindex].actualPid,SIGUSR1);*/
                if(pcbArray[pcbindex].remainingTime>Q)
                alarm(Q);
                else
                alarm(pcbArray[pcbindex].remainingTime);


            }
        }
    }


    printf("RR is done\n");

}
void attachSignalHandlers()
{
    // Attach signal handler to handle a process stop
    signal(SIGCHLD, processStopped);
    // Attach signal handler to recieve processes from process generator
    signal(SIGUSR1, processRecieved);
    signal(SIGUSR2, finishedGeneratingProcess);
    signal(SIGALRM,quantumFinished);

}
void processRecieved(int signum) {
    recieveProcess();
    
    // TODO: Use the priority ready queue instead of normal queue (and with pcb)
    enqueue(readyQueue, pcbArray[pcbArraySize].processData);
    pcbArraySize++;
    printQueue(readyQueue);

}
void quantumFinished(int signum)
{
    if(isProcessRunning)
    {
     int actualpid=pcbArray[getPCBIndex(peak(readyQueue)->pData.id)].actualPid;
    kill(actualpid,SIGUSR1);
    }

}
