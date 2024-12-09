#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

// Function to find the waiting time for all processes in Priority Scheduling
void findWaitingTimePriority(ProcessType plist[], int n) {
    
    qsort(plist, n, sizeof(ProcessType), my_comparer);
  
    
    findWaitingTime(plist, n);
}

// Function to sort the Process based on priority
int my_comparer(const void *this, const void *that) {
    ProcessType *a = (ProcessType *)this;
    ProcessType *b = (ProcessType *)that;
    
    
    return a->pri - b->pri;
}

// Function to find the waiting time for all processes in Shortest Job First (SJF)
void findWaitingTimeSJF(ProcessType plist[], int n) {
    int remaining_time[n];  // Array to store remaining burst times
    for (int i = 0; i < n; i++) {
        remaining_time[i] = plist[i].bt;
    }

    int t = 0;  
    int completed = 0;  

    while (completed < n) {
        int idx = -1;
        int min_time = INT_MAX;
        
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && plist[i].bt < min_time) {
                min_time = plist[i].bt;
                idx = i;
            }
        }

        if (idx != -1) {
            remaining_time[idx]--;
            if (remaining_time[idx] == 0) {
                completed++;
                plist[idx].wt = t - plist[idx].bt;  
            }
        }
        t++; 
    }
}

// Function to find the waiting time for all processes in Round Robin (RR)
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) {
    int remaining_time[n];
    for (int i = 0; i < n; i++) {
        remaining_time[i] = plist[i].bt;
        plist[i].wt = 0;  
    }

    int t = 0;  
    int completed = 0;  

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] > quantum) {
                    t += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    t += remaining_time[i];
                    plist[i].wt = t - plist[i].bt;
                    remaining_time[i] = 0;
                    completed++;
                }
            }
        }
    }
}

// Function to find the waiting time for all processes
void findWaitingTime(ProcessType plist[], int n) {
     
    plist[0].wt = 0 +  plist[0].art; 
  
     
    for (int  i = 1; i < n ; i++) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
}

// Function to calculate turn around time 
void findTurnAroundTime(ProcessType plist[], int n) { 
     
    for (int i = 0; i < n; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
}

// Function to calculate average time for FCFS
void findavgTimeFCFS(ProcessType plist[], int n) {
    
    findWaitingTime(plist, n);
  
    
    findTurnAroundTime(plist, n);
  
    
    printf("\n*********\nFCFS\n");
}

// Function to calculate average time for SJF
void findavgTimeSJF(ProcessType plist[], int n) {
    
    findWaitingTimeSJF(plist, n);
  
    findTurnAroundTime(plist, n);
  
    printf("\n*********\nSJF\n");
}

// Function to calculate average time for RR
void findavgTimeRR(ProcessType plist[], int n, int quantum) {
    findWaitingTimeRR(plist, n, quantum);
  
    findTurnAroundTime(plist, n);
  
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

// Function to calculate average time for Priority Scheduling
void findavgTimePriority(ProcessType plist[], int n) {
    findWaitingTimePriority(plist, n);
  
    findTurnAroundTime(plist, n);
  
    printf("\n*********\nPriority\n");
}

// Function to print the metrics: waiting time, turnaround time, and averages
void printMetrics(ProcessType plist[], int n) {
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    for (int i = 0; i < n; i++) { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
}

// Function to initialize processes from the input file
ProcessType * initProc(char *filename, int *n) {
  	FILE *input_file = fopen(filename, "r");
	if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}

// Driver code
int main(int argc, char *argv[]) { 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        return 1;
    }

    // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeFCFS(proc_list, n);
    printMetrics(proc_list, n);
  
    // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeSJF(proc_list, n);
    printMetrics(proc_list, n);
  
    // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    findavgTimePriority(proc_list, n); 
    printMetrics(proc_list, n);
    
    // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeRR(proc_list, n, quantum); 
    printMetrics(proc_list, n);
    
    return 0; 
} 
