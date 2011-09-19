#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
 
void SignHandler(int iSignNo)
{
	printf("Capture sign no:\n"); 
    printf("Capture sign no:%d\n",iSignNo); 
}
 
int main()
{
    signal(SIGINT,SignHandler); 
    while(1) 
        sleep(1); 
    return 0; 
}
