#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
	
  int PScheduler(void);
  int PIAgingScheduler(void);
  int PDAgingScheduler(void);

  printf(1, "\n This program tests the correctness of your lab#2\n"); 
  if(atoi(argv[1]) == 1) { 
    PScheduler();
  }
  else if (atoi(argv[1]) == 2) {
    PIAgingScheduler();
  }
  else if (atoi(argv[1]) == 3) {
    PDAgingScheduler();
  }
  else {
    printf(1, "\n Enter \"lab2 1\" to test Priority Scheduling\n");
    printf(1, "\n Enter \"lab2 2\" to test Aging Priority Scheduling decreasing priority feature, 3 childrene example\n");
    printf(1, "\n Enter \"lab2 3\" to test Aging Priority Scheduling decreasing prioirty feature, 1 child example \n");
  } 
  exit(0);
}

int 
PScheduler(void){
  // use this part to test the priority scheduler. Assuming that the priorities range between range between 0 to 31
  // 0 is the highest priority and 31 is the lowest priority.  
  int pid;
  int i,j,k;
  printf(1, "\n  Step 2: testing the priority scheduler and setpriority(int priority)) systema call:\n");
  printf(1, "\n  Step 2: Assuming that the priorities range between range 0 to 31\n");
  printf(1, "\n  Step 2: 0 is the highest priority. All processes have a default priority of 10\n");
  printf(1, "\n  Step 2: The parent processes will switch to priority 0\n");
  setpriority(31);
  for (i = 0; i <  3; i++) {
    pid = fork();
    if (pid > 0 ) {
      continue;
    }
    else if ( pid == 0) {
      setpriority(30-10*i);	
      for (j=0;j<50000;j++) {
        for(k=0;k<1000;k++) {
	  asm("nop"); 
        }
      }
      printf(1, "\n child# %d with priority %d has finished! \n",getpid(),30-10*i);		
      exit(0);
    }
    else {
      printf(2," \n Error \n");			
    }
  }
  if(pid > 0) {
    for (i = 0; i <  3; i++) {
      wait(0);
    }
    printf(1,"\n if processes with highest priority finished first then its correct \n");
  }
  exit(0);		
  return 0;
}

int
PIAgingScheduler(void) {

  int pid;
  int i;
  printf(1, "\n  This will now test aging of priority. If a process waits increase its priority. If a process runs decreases its priority \n");
  setpriority(31);
  for (i = 0; i < 3; i++) {
    pid = fork();
    if (pid > 0) {
      continue;
	}
    else if (pid == 0) {
      setpriority(30 - 10*i);
      printf(1, "\n child# %d with original priority %d and new priority %d has finished! \n", getpid(), 30 - 10*i, getpriority());
      exit(0);
    }
    else {
      printf(2, "\n Error \n");
      exit(1);
    }
  }
  if(pid > 0) {
    //for (i = 0; i < 3; i++) {
	while(wait(0) > 0) {
      	printf(1, "\n Parent# %d waiting with priority %d \n", getpid(), getpriority());
	}
      printf(1, "\n Parent#%d exits with priority %d \n", getpid(), getpriority());
	//exit(0);	
	//}
    printf(1, "\n Child processes should all finish first. Parent process will wait until all children exit then exit.\n");
    printf(1, "\n Correct if parent process will decrement in priority then exit.\n");
  }
  exit(0);
  return 0;
}

int
PDAgingScheduler(void) {
  
  printf(1, "\n This will now test aging of priorirty. If a process waits increase its priority. If a process runs decreases its priority\n");
  int pid;
  setpriority(31);
  pid = fork();
  if(pid > 0) {
    setpriority(15);
    printf(1,"\n This is parent process %d with priority %d \n", getpid(), getpriority());
    wait(0); 
    printf(1,"\n Parent# %d with original priority %d and new priority %d has finished! \n", getpid(), 15, getpriority());
  }
  else if (pid == 0) {
    setpriority(10);
    printf(1, " \n This is child process %d with priority %d \n ", getpid(), getpriority() );
    printf(1,"\n Child# %d with original priority %d and new priority %d has finished! \n", getpid(), 10, getpriority());
    exit(0);     
  }
  else {
    printf(1,"\n Error \n");
  }
  printf(1, "\n If the parent process decrements by 1 then it is correct! \n"); 
  exit(0); 
  return 0;
}
