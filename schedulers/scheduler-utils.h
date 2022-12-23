#pragma once
#include "../headers.h"
#include "../DataStructures/Queue.h"
#include "../ProcessStructs/PCB.h"

#define MAX_PROCESSES 100

extern struct PCB pcbArray[MAX_PROCESSES];
extern int pcbArraySize;
extern struct Queue* readyQueue;


extern bool isFinishedGenerating;
extern bool isProcessRunning;
extern int msgQueueId;

void recieveProcess();
int runProcess(int runningTime);
int runNextProcess();
int getPCBIndex(int pid);

void attachSignalHandlers();
void processRecieved(int signum);
void processStopped(int signum);
void finishedGeneratingProcess(int signum);