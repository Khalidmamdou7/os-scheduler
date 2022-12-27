#include "PCB.h"

struct PCB initializePCB(struct ProcessData processData) {
    struct PCB pcb;
    pcb.pid = processData.id;
    pcb.processData = processData;
    pcb.state = READY;
    pcb.startTime = -1;
    pcb.finishTime = -1;
    pcb.remainingTime = processData.runningTime;
    pcb.waitingTime = 0;
    pcb.turnaroundTime = 0;
    pcb.weightedTurnaroundTime = 0;
    
    return pcb;
}