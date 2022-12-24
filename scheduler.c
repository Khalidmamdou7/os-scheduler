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

    switch (schAlg)
    {
        case SJF:
        {
            printf("Scheduler started with SJF scheduling algorithm\n");
            char* args[] = { "./build/scheduler-sjf.out", NULL };
            execv(args[0], args);
        }
        case HPF:
        {
            printf("Scheduler started with HPF scheduling algorithm\n");
            char* args[] = { "./build/scheduler-hpf.out", NULL };
            execv(args[0], args);
        }
        case RR:
        {
            if (argc < 5)
            {
                printf("Error: Quantum time is not specified\n");
                printf("Run the program as: ./scheduler.out -sch <scheduling_algorithm_number> -q <quantum>\n");
                exit(1);
            }
            printf("Scheduler started with RR scheduling algorithm\n");
            char* args[] = { "./build/scheduler-rr.out", argv[4], NULL };
            execv(args[0], args);
        }
        case MLF:
        {
            if (argc < 5)
            {
                printf("Error: Quantum time is not specified\n");
                printf("Run the program as: ./scheduler.out -sch <scheduling_algorithm_number> -q <quantum>\n");
                exit(1);
            }
            printf("Scheduler started with MLF scheduling algorithm\n");
            char* args[] = { "./build/scheduler-mlf.out", argv[4], NULL };
            execv(args[0], args);
        }
        default:
            printf("Error: Scheduling algorithm value is not valid\n");
            break;
    }

}