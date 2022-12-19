#include "headers.h"
#include "./DataStructures/Queue.h"
#include "enums.h"
#include "./ipc/MsgStruct.h"
#include "./ipc/MsgQueue.h"

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
    if (argc < 4)
    {
        printf("Error: Scheduling algorithm is not specified\n");
        printf("Run the program as: ./process_generator <input_file_path> -sch <scheduling_algorithm_number> <scheduling_algorithm_parameters>\n");
        exit(1);
    }
    enum SchedulingAlgorithm schAlg = atoi(argv[3]);
    // print the chosen scheduling algorithm string
    printf("Scheduling algorithm is %d\n", schAlg);


    // 3. Initiate and create the scheduler and clock processes.
    int pid = fork();
    if (pid == 0)
    {
        printf("Process generator running the clk\n");
        char* args[] = { "./build/clk.out", NULL };
        execv(args[0], args);
    } else {
        pid = fork();
        if (pid == 0)
        {
            // pass the scheduling algorithm number and its parameters to the scheduler
            char* args[] = { "./build/scheduler.out", "-sch", argv[3], NULL };
            execv(args[0], args);
        }
    }

    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function.
    int time = getClk();
    printf("Current Time is %d\n", time);
    printf("Hello from process generator\n");
    // sleep(1);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    
    // Create a unique key via call to ftok() which takes a file path and an integer identifier as its arguments.
    key_t msgKey = ftok("./ipc/keyfile", 65);
    int msgid = getMsgQueue(msgKey);
    struct MsgStruct msg = {
        .mtype = 1,
        .data = peak(q)->pData
    };
    while (!isEmpty(q))
    {
        if (msg.data.arrivalTime == getClk())
        {
            dequeue(q);
            printf("Sending message to scheduler: %d %d %d %d -- at time: %d\n",
                msg.data.id, msg.data.arrivalTime, msg.data.runningTime, msg.data.priority, getClk());
            sendMsg(msgid, msg);
            msg.data = peak(q) ? peak(q)->pData : msg.data;
            if (isEmpty(q))
                break;
        }
    }


    sleep(1);
    destroyMsgQueue(msgid);
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    destroyMsgQueue(ftok("keyfile", 65));
    destroyClk(true);
}
