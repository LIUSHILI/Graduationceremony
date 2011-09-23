/**
*  mypipeclient.c: 客户端程序
*  Author: Navins
*  Program Date: 2010-7-23
*  Modified Date: 2010-7-23
*  使用Linux的命名管道机制，用C语言编写的客户器端程序。
*  程序中使用open函数以读写方式打开命名管道，隔3s写信息。
**/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define FIFO_CHANNEL "/home/you/my_fifo"  /* 宏定义，fifo路径 */

int main()
{
 int fd;
 char s[]="Hello!";

 if((fd=open(FIFO_CHANNEL,O_WRONLY))==-1)  /* 以读写方式打开命名管道，返回-1代表失败 */
 {
  perror("Can't open the FIFO");
  return 1;
 }

 while(1)  /* 不断向管道中写信息 */
 {
  write( fd, s, sizeof(s) );
  printf("Write: %s\n",s);
  sleep(3);  /* sleep 3s */
 }

 close(fd);  /* 关闭管道 */
 return 0;
}


