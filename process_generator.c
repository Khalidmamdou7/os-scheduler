#include "headers.h"

void clearResources(int);
//---------//
struct processData
{
    int id;
    int arrivaltime;
    int runningtime;
    int priority;
};
//--------//
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization

    // 1. Read the input files.
    char* input_file=argv[1];
    struct processData pData[4];  //read processes in array index 0 arrived first
    int i = 0;
    FILE *Input_File = fopen(input_file, "r");

    // while (!feof(Input_File))
    // {
    //     fscanf(Input_File, "%d %d %d %d", &pData[i].id,&pData[i].arrivaltime,&pData[i].runningtime,&pData[i].priority);
    //     i++;
    // }
    char buff[120];
    while (fgets(buff, 120, Input_File))//problem in reading process 5
    {
        if(buff[0]=='#')//skip comment
            continue;
        sscanf(buff, "%d\t%d\t%d\t%d", &pData[i].id,&pData[i].arrivaltime,&pData[i].runningtime,&pData[i].priority);
        //printf("%d %d %d %d \n", pData[i].id,pData[i].arrivaltime,pData[i].runningtime,pData[i].priority);
        printf("%s \n", buff);
        i++;
    }
    fclose(Input_File);
    int size = i;
    for (i = 0; i < size; i++)
    {
        printf("%d\t%d\t%d\t%d\n", pData[i].id,pData[i].arrivaltime,pData[i].runningtime, pData[i].priority);
    }

    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    char *sch_algorithm = argv[3]; // till we add other parameters

    // 3. Initiate and create the scheduler and clock processes.

    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function.
    int time = getClk();
    printf("Current Time is %d\n", time);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // data struct created when we read the input file
    // 6. Send the information to the scheduler at the appropriate time.
    i = 0;
    while (true)
    {
        if (pData[i].arrivaltime == time)
        {
            ;    // pass it to shedular
            i++; // go to next process
        }
}
// 7. Clear clock resources
destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
