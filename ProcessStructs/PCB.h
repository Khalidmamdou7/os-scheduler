#ifndef PCB_H
#define PCB_H

#include "../enums.h"
#include "ProcessData.h"

struct PCB
{
    int pid;
    int actualPid;
    struct ProcessData processData;
    enum State state;
    int startTime;
    int finishTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int weightedTurnaroundTime;
    
};

struct PCB initializePCB(struct ProcessData processData);

#endif