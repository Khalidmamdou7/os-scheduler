#pragma once
#include "../enums.h"

void logState(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime);
void logFinished(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime, int turnaroundTime, float weightedTurnaroundTime);

void logPerformance(float cpuUtilization, float avgWeightedTurnaroundTime, float avgTurnaroundTime, float avgWaitingTime);

void logMemory(int time, int processId, int size, enum LogMemState state, int begin, int end);