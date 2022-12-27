#pragma once
#include "../enums.h"

void logState(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime);
void logFinished(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime, int turnaroundTime, float weightedTurnaroundTime);

void logPerformance(float cpuUtilization, float avgWeightedTurnaroundTime, float avgTurnaroundTime, float avgWaitingTime);
