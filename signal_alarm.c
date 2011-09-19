#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void sigalrm_fn(int sig)
{
        /* Do something */
        printf("alarm!\n");
        alarm(2);
        return;
}
int main(void)
{
        signal(SIGALRM, sigalrm_fn);
        alarm(2);
        /* Do someting */
        while(1) pause();
}