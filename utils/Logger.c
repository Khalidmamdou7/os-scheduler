#include "Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


void logState(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime)
{
    return logFinished(time, processId, state, arrivalTime, runTime, remainingTime, waitingTime, 0, 0.00);
}
 

void logFinished(int time, int processId, enum LogState state, int arrivalTime,
            int runTime, int remainingTime, int waitingTime, int turnaroundTime, float weightedTurnaroundTime) {
                
    FILE *file = fopen("Logs/Scheduler.log", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    printf("Logging State...\n");

    fprintf(file, "At time %d process %d %s arr %d total %d remain %d wait %d", time, processId, 
        state == STARTED ? "started" : state == STOPPED ? "stopped" : state == FINISHED ? "finished" : "resumed",
            arrivalTime, runTime, remainingTime, waitingTime);
    if (state == FINISHED) {
        fprintf(file, " TA %d WTA %.2f\n", turnaroundTime, weightedTurnaroundTime);
    } else {
        fprintf(file, "\n");
    }

    fclose(file);

}

void logPerformance(float cpuUtilization, float avgWeightedTurnaroundTime, float avgTurnaroundTime, float avgWaitingTime)
{
    FILE *file = fopen("Logs/Scheduler.perf", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    printf("Logging Performance...\n");
    fprintf(file, "CPU utilization = %.2f %\n", cpuUtilization);
    fprintf(file, "Avg WTA = %.2f\n", avgWeightedTurnaroundTime);
    fprintf(file, "Avg TA = %.2f\n", avgTurnaroundTime);
    fprintf(file, "Avg WT = %.2f\n", avgWaitingTime);

    fclose(file);
}

void logMemory(int time, int processId, int size, enum LogMemState state, int begin, int end)
{
    FILE *file = fopen("Logs/Memory.log", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    printf("Logging Memory...\n");

    fprintf(file, "At time %d process %d %s %d %d-%d\n", time, processId, 
        state == ALLOCATED ? "allocated" : state == DEALLOCATED ? "deallocated" : "failed", size, begin, end);

    fclose(file);
}

// int main() {
//     // logState(1, 1, STARTED, 1, 4, 4, 0);
//     // logFinished(5, 1, FINISHED, 1, 4, 0, 0, 4, 1.00);
//     // logState(5, 2, STARTED, 2, 3, 3, 0);
//     // logFinished(8, 2, FINISHED, 2, 3, 0, 0, 6, 2.00);
//     logPerformance(0.50, 1.50, 5.00);
//     logPerformance(0.50, 1.50, 5.00);
//     return 0;
// }