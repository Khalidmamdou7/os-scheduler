#include "headers.h"
#include "ProcessData.h"
#include "Queue.c"

void clearResources(int);

int main(int argc, char *argv[])
{
    // signal(SIGINT, clearResources);
    // TODO Initialization

    // 1. Read the input files.
    char* filePath = argv[1];
    FILE* inpFilePtr = fopen(filePath, "r");

    if (inpFilePtr == NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    struct Queue* q = createQueue();

    char buff[120];
    while (fgets(buff, 120, inpFilePtr))//problem in reading process 5
    {
        if(buff[0]=='#')//skip comment
            continue;
        
        struct ProcessData p;
        sscanf(buff, "%d %d %d %d", &p.id, &p.arrivalTime, &p.runningTime, &p.priority);
        enqueue(q, p);
    }
    printQueue(q);
    fclose(inpFilePtr);

    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function.
    int time = getClk();
    printf("Current Time is %d\n", time);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
