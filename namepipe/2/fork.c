#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
 char buf[80];
 int fd;
 pid_t pid;
 unlink( "fifo" );
 mkfifo( "fifo", 0777 );

 if( (pid=fork()) == 0 )
 {
  char s[] = "Hello!\n";
  fd = open( "fifo", O_WRONLY );  
  while(1)
  {
   write( fd, s, sizeof(s) );
   sleep(3);
  }
  close( fd );
 }
 else if( pid > 0 )
 {
  fd = open( "fifo", O_RDONLY );
  while(1)
  {
   read( fd, buf, sizeof(buf) );
   printf("Message from Client: %s\n",buf );
   sleep(3);
  }
  close( fd );
 }
 else
 {
  printf("Error");
 }
 return 0;
}


