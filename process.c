#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    initClk();
    int timer = getClk();

    //TODO The process needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    //remainingtime = ??;
    printf("A process has been created at time %d and it has remaining time %d\n", getClk(), remainingtime);
    while (remainingtime > 0)
    {
        if (timer != getClk())
        {
            timer = getClk();
            remainingtime--;
            printf("A process has been created at time %d and it has remaining time %d\n", getClk(), remainingtime);
        }
        
    }

    printf("Process %d finished at time %d\n", getpid(), getClk());

    return 0;
}
