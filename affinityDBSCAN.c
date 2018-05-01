// Joseph Moreno
// CSC 251
// 4/30/18
// DBSCAN

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define minPoints 7


struct neighbor {
    int xPos;
    int yPos;
    int neighborAmount;
    int label; //0 is noise; 1 is border; 2 is core
};

double stdDistance(int xOG, int yOG, int xComp, int yComp);
void rangeQuery(struct neighbor dataSet[], int count);
int main(int argc, char *argv[])
{
    int eps = atoi(argv[2]);
    int c = 0, i = 0, j = 0,xTemp = 0, yTemp = 0;
    int arrCount = 0;
    FILE *dataPointer;
    FILE *wptr;
    FILE *clust;
    char string[256];
    if ((dataPointer = fopen(argv[1], "r")) == NULL)
    {
        printf("%s does not exist.", argv[1]);
    }
    while((fgets(string, 256, dataPointer)) != NULL)
    {
        printf("%d \n", arrCount);
        arrCount++;
    }
    struct neighbor dataSet[arrCount];
    rewind(dataPointer);
    for (i = 0; i < arrCount; i++)
    {
        fscanf(dataPointer, "%s", string);
        xTemp = atoi(strtok(string, ","));
        yTemp = atoi(strtok(NULL, ","));
        dataSet[i].xPos = xTemp;
        dataSet[i].yPos = yTemp;
    }

    for(i = 0; i < arrCount; i++)
    {
        for(j = 0; j < i; j++)
        {
            if (stdDistance(dataSet[i].xPos, dataSet[i].yPos, dataSet[j].xPos, dataSet[j].yPos) <= (double) eps)
            {
                dataSet[i].neighborAmount++;
                
                if (dataSet[i].label < 2)
                    dataSet[i].label = 1;
                if (dataSet[j].label < 2)
                    dataSet[j].label = 1;
            }
        }
        if (dataSet[i].neighborAmount >= minPoints)
        {
            dataSet[i].label = 2;
        }
    }

    wptr = fopen("output.txt", "w");
    for (i = 0; i < arrCount; i++)
    {
        fprintf(wptr, "[%d,%d] Neightbors: %d; Label: %d \n", dataSet[i].xPos, dataSet[i].yPos, dataSet[i].neighborAmount, dataSet[i].label);
    }
    
    
    for (i = 0; i <arrCount; i++)
    {
        if (dataSet[i].label == 2)
        {
            char clustername[256] = "cluster%d.txt",i;
            clust = fopen(clustername, "w"); 
            fprintf(clust, "Cluster ID %d; Epsilon = %d \n", eps, i);   
            fprintf(clust, "[%6d,%6d] \n", dataSet[i].xPos, dataSet[i].yPos);        
            for(j = 0; j < i; j++)
            {
                if (stdDistance(dataSet[i].xPos, dataSet[i].yPos, dataSet[j].xPos, dataSet[j].yPos) <= (double) eps)
                {
                    fprintf(clust,"[%6d,%6d] \n", dataSet[j].xPos, dataSet[j].yPos);
                }
            }
            fclose(clust);
        }
    }
    fclose(wptr);
    return 0; 
}

double stdDistance(int xOG, int yOG, int xComp, int yComp)
{
    double newX = (double) xComp - (double) xOG;
    double newY = (double) yComp - (double) yOG;
    double xSquare = newX * newX;
    double ySquare = newY * newY;
    double checkSquare = xSquare + ySquare;
    return sqrt(checkSquare);
}
