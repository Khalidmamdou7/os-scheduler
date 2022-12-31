#pragma once
#include "../headers.h"
#include "../ProcessStructs/PCB.h"

#define MAX_PROCESSES 100

extern struct PCB pcbArray[MAX_PROCESSES];
extern int pcbArraySize;


extern bool isFinishedGenerating;
extern bool isProcessRunning;
extern int msgQueueId;

void recieveProcess();
int runProcess(int runningTime);
int runNextProcess(struct ProcessData* pData);
int getPCBIndex(int pid);
int getPCBIndexByActualPid(int actualPid);

void finishedGeneratingProcess(int signum);
//memory allocation
int getSuitableSize(int process_size);
int allocateMem(int process_size,int process_id);
int deallocateMem(int process_size,int process_id,int starting_address);