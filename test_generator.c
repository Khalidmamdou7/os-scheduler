#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./ProcessStructs/ProcessData.h"
#define null 0

int main(int argc, char *argv[])
{
    FILE *pFile;
    pFile = fopen("processes.txt", "w");
    int no;
    struct ProcessData pData;
    printf("Please enter the number of processes you want to generate: ");
    scanf("%d", &no);
    srand(time(null));
    //fprintf(pFile,"%d\n",no);
    fprintf(pFile, "#id arrival runtime priority memsize\n");
    pData.arrivalTime = 1;
    for (int i = 1; i <= no; i++)
    {
        //generate Data Randomly
        //[min-max] = rand() % (max_number + 1 - minimum_number) + minimum_number
        pData.id = i;
        pData.arrivalTime += rand() % (11); //processes arrives in order
        pData.runningTime = rand() % (30);
        pData.priority = rand() % (11);
        pData.size = rand() % (257);
        fprintf(pFile, "%d\t%d\t%d\t%d\t%d\n", pData.id, pData.arrivalTime, pData.runningTime, pData.priority, pData.size);
    }
    fclose(pFile);
}
