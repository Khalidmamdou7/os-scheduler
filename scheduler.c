#include "headers.h"
#include "enums.h"
#include "./ipc/MsgStruct.h"
#include "./ipc/MsgQueue.h"
#include "./ProcessStructs/PCB.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100



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

    printf("Scheduler started with %d scheduling algorithm\n", schAlg);

    // Create arrays to store pcbs
    struct PCB pcbArray[MAX_PROCESSES];
    int pcbArraySize = 0;

    key_t msgKey = ftok("./ipc/keyfile", 65);
    int msgQueueId = getMsgQueue(msgKey);

    while (1)
    {
        struct MsgStruct msg = receiveMsg(msgQueueId, 1);
        printf("Received message from process generator: %d %d %d %d\n", msg.data.id, msg.data.arrivalTime, msg.data.runningTime, msg.data.priority);
        if (msg.data.id == -1)
        {
            break;
        }
        

        pcbArray[pcbArraySize++] = initializePCB(msg.data);
        printf("PCB created with id: %d\n", pcbArray[pcbArraySize - 1].pid);
        
        if (pcbArraySize == MAX_PROCESSES)
        {
            printf("Error: Reached maximum number of processes\n");
            exit(1);
        }

        // Start the process
        int pid = fork();
        if (pid == 0)
        {
            printf("Forked process with id: %d\n", pcbArray[pcbArraySize - 1].pid);
            char* runningTime = malloc(10);
            sprintf(runningTime, "%d", msg.data.runningTime);
            char* args[] = { "./build/process.out", runningTime, NULL };
            execv(args[0], args);
        } else
        {
            // wait for the process to finish
            int status;
            waitpid(pid, &status, 0);
            printf("Process %d finished at time %d\n", pid, getClk());
        }
        

    }
    


    //TODO: implement the scheduler.
    // The scheduler should be able to:
    // 1.Start a new process whenever it arrives. (Fork it and give it its parameters)
    // 2.Switch between two processes according to the scheduling algorithm. (stop
    // the old process and save its state and start/resume another one.)
    // 3.Keep a process control block (PCB) for each process in the system. A PCB
    // should keep track of the state of a process; running/waiting, execution time,
    // remaining time, waiting time, etc.
    // 4.Delete the data of the process when it finishes its job. The scheduler knows
    // that a process has finished if its remaining time reaches zero. Note that the
    // scheduler does NOT terminate the process. It just deletes its data from the
    // process control block and its data structures.
    // 5.Report the following information:
    // a) CPU utilization.
    // b) Average Weighted Turnaround Time
    // c) Average Waiting Time
    // 6.Generate two files: (check the input/output section
    // below)(a)Scheduler.log
    // (b) Scheduler.perf



    //TODO: upon termination release the clock resources.

    printf("Scheduler terminated\n");
    // destroyClk(true);
}
