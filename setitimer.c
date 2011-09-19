#include <stdio.h>    // for printf()  
#include <unistd.h>   // for pause()  
#include <signal.h>   // for signal()  
#include <string.h>   // for memset()  
 
#include <sys/time.h> // struct itimeral. setitimer()   
  
void printMsg(int); 
unsigned second =0; 
 
int main() 
{  
  
 // Get system call result to determine successful or failed   
  
  int res = 0;  
  
  // Register printMsg to SIGALRM   
  
  signal(SIGALRM, printMsg);  
  
   
 
  struct itimerval tick;  
  
  // Initialize struct   
  
 memset(&tick, 0, sizeof(tick));  
 
 // Timeout to run function first time   
 
 tick.it_value.tv_sec = 0;  // sec   
 
 tick.it_value.tv_usec = 10000; // micro sec.   
  
  // Interval time to run function   
 
  tick.it_interval.tv_sec = 0;  
 
  tick.it_interval.tv_usec = 10000;  
  
 // Set timer, ITIMER_REAL : real-time to decrease timer,   
  
  //                          send SIGALRM when timeout   
  
  res = setitimer(ITIMER_REAL, &tick, NULL);  
 
  if (res)
  {  
    printf("Set timer failed!!/n");  
  }  
  // Always sleep to catch SIGALRM signal   
  
  while(1) {  
  
    pause();  
  
  }  
 
 
 
 return 0;    
  
}  

 
 
void printMsg(int num) 
{  
	second++;

	printf("time:%d-%d-%d\n",second/100,(second/10)%10,(second%100)%10);
	if(second == 6000)
	second = 0;
	
  
}  