//
//
//  Implementation of First-Come First-Served
//  preemptive Shortest Job First, and Round-Robin
//  CPU scheduling algorithms
//
//
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Data struct for all pertinant information regarding processes
typedef struct datastruct
{
	int processCount, runFor, quantum, arrival[50], burst[50];
	char scheduleType[4], processName[50][20]; 	
} Data;

Data readIn(char fileName[]);
void fcfs(Data data);
void sjf(Data data);
void rr(Data data);
int arrivalTimeMin(int [], int [], int);


int main()
{
	char fileName[20];
	printf("Please enter the file name: ");
	scanf("%s", fileName);
	Data data = readIn(fileName);

	switch (data.scheduleType[0]) 
	{
		case 'f': 
			fcfs(data);
			break;
		case 's': 
			sjf(data);
			break;
		case 'r': 
			rr(data);
			break;
		default:
			break;
	}

	return 0;


}

void fcfs(Data data)
{
	FILE *fileOut;

	fileOut = fopen("processes.out","w");
	
	fprintf(fileOut,"%d processes\n", data.processCount);
	fprintf(fileOut,"Using First-Come First-Served\n\n");	

	int minimum = 10000;
	int minimumIndex = 1;
	int minimumIndex_new = 0;
	int time= 0;
	int i;

	int tracker[data.processCount][2];
	int processCounter = data.processCount;

	
	while (time <= data.runFor) 
	{
		for (i = 0; i < data.processCount; i++) 
		{
			if (data.arrival[i] == time) 
			{
				fprintf(fileOut,"Time %d: %s arrived\n",time,data.processName[i]);
			}
		}
		minimumIndex_new = arrivalTimeMin(data.arrival, data.burst, data.processCount);
		if(minimumIndex_new != minimumIndex)
		{
			fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, data.processName[minimumIndex_new], data.burst[minimumIndex_new]);
			minimumIndex = minimumIndex_new;
			tracker[minimumIndex][0] = time-data.arrival[minimumIndex];
		}

		data.burst[minimumIndex]--;
		time ++;
		if (data.burst[minimumIndex] == 0) 
		{
			fprintf(fileOut,"Time %d: %s finished\n",time,data.processName[minimumIndex]);
			tracker[minimumIndex][1] = time-data.arrival[minimumIndex];
			processCounter--;
		}

		if (processCounter == 0) 
		{
			fprintf(fileOut,"Finished at time %d\n\n", time);
			for (i = 0; i < data.processCount; i++) 
			{
				fprintf(fileOut,"%s wait %d turnaround %d\n",data.processName[i],tracker[i][0],tracker[i][1]);
			}
		
			
			break;
		}
		
	}
	fclose(fileOut);
	
}

int arrivalTimeMin(int arrival[], int burst[], int processCount)
{
	int i;
	int minimum = 10000;
	int minimumIndex = 0;
	for (i = 0; i < processCount; i++) 
	{
		if(arrival[i] < minimum && burst[i] > 0)
		{
			minimum = arrival[i];
			minimumIndex = i;
		}
	}
	return minimumIndex;
}

void sjf(Data data)
{
	
}

void rr(Data data)
{
	
}


Data readIn(char fileName[])
{
	//Create file and initialize pertinant variables
	FILE *fileIn;
	Data data;
	int processCount = 0, runFor = 0, quantum = 0, i = 0;
	char scheduleType[4], buffer[80], labelBuffer[80], comments[80];

	//Open file and determine if it is present
	fileIn = fopen(fileName,"r");
	if (fileIn == NULL) 
	{
		printf("Could not find file.\n");
		exit(-1);
	}

	//read in 80 characters from a line. This is assumed to be the max on a line.
	//Might need to increase if errors found
	fgets(buffer, 80, fileIn);
	sscanf(buffer, "%s %d %80s", labelBuffer, &processCount, comments);
	fgets(buffer, 80, fileIn);
	sscanf(buffer, "%s %d %80s", labelBuffer, &runFor, comments);
	fgets(buffer, 80, fileIn);
	sscanf(buffer, "%s %s %80s", labelBuffer, scheduleType, comments);
	fgets(buffer, 80, fileIn);
	//If in round-robin mode, go ahead and sscanf this line, otherwise it is not necessary
	if(buffer[0] != '#')
	{
		sscanf(buffer, "%s %d %80s", labelBuffer, &quantum, comments);
	}

	//Declaring more variables now that the process count is known
	char processName[processCount][20];
	int arrival[processCount], burst[processCount];

	//Scan in individual process information including name, arrival time, and burst time
	for (i = 0; i < processCount; i++) 
	{
		fgets(buffer, 80, fileIn);
		sscanf(buffer, "%s %s %s %s %d %s %d", labelBuffer, labelBuffer, processName[i], labelBuffer, &arrival[i], labelBuffer, &burst[i]);
	}

	//Fill in Data struct information
	data.processCount = processCount;
	data.runFor = runFor;
	data.quantum = quantum;
	strcpy(data.scheduleType, scheduleType);

	for (i = 0; i < processCount; i++) 
	{
		strcpy(data.processName[i], processName[i]);
		data.arrival[i] = arrival[i];
		data.burst[i] = burst[i];
	}
	
	fclose(fileIn);

	return data;
}


