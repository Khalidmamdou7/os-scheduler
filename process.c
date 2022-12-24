#include "headers.h"

void stopHandler(int);

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    // Attach signal handler to handle a process stop (A preemption from the scheduler)
    signal(SIGSTOP, stopHandler);

    initClk();
    int startTime = getClk();
    int timer = startTime;


    //TODO The process needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    //remainingtime = ??;
    printf("A process has been created at time %d and it has remaining time %d\n", startTime, remainingtime);
    while (remainingtime > 0)
    {
        if (timer != getClk())
        {
            timer = getClk();
            remainingtime--;
            printf("A process has been created at time %d and it has remaining time %d\n", startTime, remainingtime);
        }
        
    }

    printf("Process %d finished at time %d\n", getpid(), getClk());

    return remainingtime;
}

void stopHandler(int signum)
{
    printf("A process has been stopped at time %d and it has remaining time %d\n", getClk(), remainingtime);
    exit(remainingtime);
}
