// 
//
//  Implementation of First-Come First-Served
//  preemptive Shortest Job First, and Round-Robin
//  CPU scheduling algorithms
//
//  Group 18
//
//  Jacob Garman
//	Ian Lasky
//	Sean Reedy
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
//	char fileName[20];
//	printf("Please enter the file name: ");
//	scanf("%s", fileName);
	Data data = readIn("process.in");

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
	
	//Print out first couple lines
	fprintf(fileOut,"%d processes\n", data.processCount);
	fprintf(fileOut,"Using First Come First Served\n\n");	

	//Declare and initialize variables
	int minimum = 10000;
	int minimumIndex = 1;
	int minimumIndex_new = 0;
	int time= 0;
	int i;

	//Tracker has two values for each process: wait time = time selected - arrival time
	//And turnaround time = time completed - arrival time 
	int tracker[data.processCount][2];
	int processCounter = data.processCount;

	
	//Continue while the time is still within the runFor parameter
	while (time <= data.runFor) 
	{
		//Cycle through the processes and see if they have arrived
		for (i = 0; i < data.processCount; i++) 
		{
			if (data.arrival[i] == time) 
			{
				fprintf(fileOut,"Time %d: %s arrived\n",time,data.processName[i]);
			}
		}
		//Use helper function to determine the lowest arrival time
		minimumIndex_new = arrivalTimeMin(data.arrival, data.burst, data.processCount);
		//If it has changed, then change which process should be selected
		if(minimumIndex_new != minimumIndex)
		{
			fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, data.processName[minimumIndex_new], data.burst[minimumIndex_new]);
			minimumIndex = minimumIndex_new;
			//New process selected, so edit wait time tracking parameter
			tracker[minimumIndex][0] = time-data.arrival[minimumIndex];
		}

		//Decrement this process's burst by one and increment the time by one
		data.burst[minimumIndex]--;
		time ++;
		//If this process's burst has completed print out that it has finished,
		//Edit the turn around time tracking parameter, and decrement the available
		//Process counter
		if (data.burst[minimumIndex] == 0) 
		{
			fprintf(fileOut,"Time %d: %s finished\n",time,data.processName[minimumIndex]);
			tracker[minimumIndex][1] = time-data.arrival[minimumIndex];
			processCounter--;
		}

		//If all processes have completed, print out all wait times and turn around times
		//For the processes
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
	for (i = 0; i < data.processCount; i++) 
	{
		if (data.burst[i] != 0) 
		{
			fprintf(fileOut, "%s did not finish\n", data.processName[i]);
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
	FILE *fileOut;

	fileOut = fopen("processes.out","w");
	
	fprintf(fileOut,"%d processes\n", data.processCount);
	fprintf(fileOut,"Using Shortest Job First (Pre)\n\n");	

	int i; 
	int minTime;
	int minIndex;
	int currentJob=-1; 
	int remainingJobs = data.processCount; 
	int time=0; 
	int wait[data.processCount];
	int turnaround[data.processCount]; 
	int burstTime[data.processCount]; // records original burst time for each process

	for(i=0; i<data.processCount; i++)
	{
		burstTime[i] = data.burst[i];
	 }
	
	while (time < data.runFor)
	{

	       minTime=1000; // time of shortest job 
	       minIndex=-1; // index of shortest job  
		for(i=0; i<data.processCount; i++)
		{
		    if ( data.arrival[i] == time ) // find arriving processes 
		    {
			fprintf(fileOut,"Time %d: %s arrived\n", time, data.processName[i]);
		    }
		    if (data.burst[i] <= minTime && data.burst[i]>0 && data.arrival[i] <= time)  // find shortest process that hasn't finished and has arrived   
		    {
			minIndex = i; 
			minTime = data.burst[i]; 
		    }
		}
		
		if (minIndex == -1) // no process can run -> IDLE 
		{
		    fprintf(fileOut,"Time %d: IDLE\n", time); 
		    time++;
		}
		else
		{
			if (minIndex != currentJob) // if new process selected 
			{
				fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, data.processName[minIndex], data.burst[minIndex]);
		    	}
			currentJob = minIndex; 
			data.burst[currentJob]--; 
			time++; 
			if ( data.burst[currentJob] == 0 ) // job finished 
			{
			    fprintf(fileOut,"Time %d: %s finished\n", time, data.processName[currentJob]); 
			    wait[currentJob] = time - (data.arrival[currentJob] + burstTime[currentJob]);
			    turnaround[currentJob] = time - data.arrival[currentJob]; 
			    remainingJobs--; 
			}
		}
		

	}

	if ( remainingJobs == 0 ) // success 
	{
	    fprintf(fileOut,"Finished at time %d\n\n", time);
	}
	else
	{
	    fprintf(fileOut, "Did not finish (time %d)\n\n", time);
	}

	for(i=0; i<data.processCount; i++) // output wait and turnaround times, or state process did not finish 
	{
	    if ( data.burst[i] == 0 ) 
		fprintf(fileOut,"%s wait %d turnaround %d\n", data.processName[i], wait[i], turnaround[i]);
	    else 
		fprintf(fileOut, "%s did not finish\n", data.processName[i]); 
	}

	fclose(fileOut); 
}

void rr(Data data)
{
	FILE *fileOut;

	fileOut = fopen("processes.out","w");
	
	fprintf(fileOut,"%d processes\n", data.processCount);
	fprintf(fileOut,"Using Round-Robin\n");	
	fprintf(fileOut,"Quantum %d\n\n", data.quantum);

	int ready[1024];
	int qhead=0, qtail=0;
	int time= 0;
	int active = -1;
	int remaining = 0;
	int i;

	int finished[50];
	//need to preserve the original duration, because wait is end_time - start_time - run_time
	int duration[50];
	for(i=0; i<data.processCount; i++)
		duration[i] = data.burst[i];
	int processCounter = data.processCount;

	
	while (time <= data.runFor) 
	{
		//check if any processes arrive this tick
		for (i = 0; i < data.processCount; i++) 
		{
			if (data.arrival[i] == time) 
			{
				fprintf(fileOut,"Time %d: %s arrived\n",time,data.processName[i]);
				ready[qtail++] = i;
			}
		}
		
		//Context switch
		if(remaining <= 0) {
			//if not idle			
			if(active >= 0) {
				ready[qtail++] = active;
				active = ready[qhead++];
				remaining = data.quantum;
				fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, data.processName[active], data.burst[active]);
			} else {
				if(qtail != qhead) {
					active = ready[qhead++];
					remaining = data.quantum;
					fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, data.processName[active], data.burst[active]);
				} else {
					fprintf(fileOut, "Time %d: IDLE\n", time);
				}
			}
		}
		
		//simulation update
		data.burst[active]--;
		time++;
		remaining--;
		
		//wrap up a completed process
		if (data.burst[active] == 0) 
		{
			fprintf(fileOut,"Time %d: %s finished\n",time,data.processName[active]);
			finished[active] = time;
			active = -1;
			processCounter--;
			remaining = 0;
		}
		
		

		if (time == data.runFor) 
		{
			fprintf(fileOut,"Finished at time %d\n\n", time);
			for (i = 0; i < data.processCount; i++) 
			{
				if(data.burst[i] == 0)//process finished
					fprintf(fileOut,"%s wait %d turnaround %d\n",data.processName[i],finished[i] - data.arrival[i] - duration[i],finished[i] - data.arrival[i]);
				else//process did not finish
					fprintf(fileOut,"%s did not finish.\n",data.processName[i]);
			}
			break;
		}
		
	}
	fclose(fileOut);
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


