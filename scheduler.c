#include "headers.h"
#include "enums.h"
#include "./ipc/MsgStruct.h"
#include "./ipc/MsgQueue.h"
#include "./ProcessStructs/PCB.h"
#include "./DataStructures/Queue.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

int msgQueueId;
struct PCB pcbArray[MAX_PROCESSES];
int pcbArraySize = 0;

struct Queue* readyQueue;


void processStopped(int);
void processRecieved(int);
void finishedGeneratingProcess(int);

void recieveProcess();
int runProcess(int);

bool isFinishedGenerating = false;

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

    key_t msgKey = ftok("./ipc/keyfile", 65);
    msgQueueId = getMsgQueue(msgKey);
    printf("Scheduler started with %d scheduling algorithm\n", schAlg);



    // Attach signal handler to handle a process stop
    signal(SIGCHLD, processStopped);
    // Attach signal handler to recieve processes from process generator
    signal(SIGUSR1, processRecieved);
    signal(SIGUSR2, finishedGeneratingProcess);

    readyQueue = createQueue();

    // TODO: Run the first process
    runProcess(peak(readyQueue)->pData.runningTime);

    while (!isFinishedGenerating || isEmpty(readyQueue)) // TODO: Add a condition to check whether there are still processes to run
    {
        sleep(1);
    }

    


    // TODO:
    // 1.Report the following information:
    // a) CPU utilization.
    // b) Average Weighted Turnaround Time
    // c) Average Waiting Time
    // 2.Generate two files: (check the input/output section
    // below)(a)Scheduler.log
    // (b) Scheduler.perf


    printf("Scheduler terminated\n");
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
    int pid = fork();
    if (pid == 0)
    {
        // Child process
        char remainingTimeStr[10];
        sprintf(remainingTimeStr, "%d", runningTime);
        char* args[] = {"./process.out", remainingTimeStr, NULL};
        execvp(args[0], args);
    }
    return pid;

}

void processStopped(int signum)
{
    printf("A process has stopped\n");
    // TODO: Get the finished process id and its remaining time
    int remainingTime, pid;
    pid = wait(&remainingTime);
    // TODO: Find the process in the pcb array
    int i;
    for (i = 0; i < pcbArraySize; i++)
    {
        if (pcbArray[i].actualPid == pid)
        {
            break;
        }
    }
    // TODO: Remove the process from the ready queue and insert it again if it has remaining time (preemptive)
    dequeue(readyQueue);
    // TODO: Update the process state
    // TODO: Update the process statistics
    // TODO: Log the process termination and statistics
    // TODO: Delete the process from the pcb array if it has no remaining time


    // TODO: Run the next process in the ready queue
    pid = runProcess(pcbArray[i].processData.runningTime);
    pcbArray[i].actualPid = pid;
    pcbArray[i].state = RUNNING;




}

void finishedGeneratingProcess(int signum)
{
    isFinishedGenerating = true;
}