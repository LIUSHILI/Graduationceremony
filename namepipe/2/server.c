/**
*  mypipeserver.c: 服务器端程序
*  Author: Navins
*  Program Date: 2010-7-23
*  Modified Date: 2010-7-23
*  使用Linux的命名管道机制，用C语言编写的服务器端程序。
*  程序中使用mkfifo函数创建命名管道，以只读方式open命名管道，隔3s读信息。
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
 char buf[80];
 
 if(mkfifo(FIFO_CHANNEL,0777)==-1) /* 创建命名管道，返回-1表示失败 */
 {
  perror("Can't create FIFO channel");
  return 1;
 }
 
 if((fd=open(FIFO_CHANNEL,O_RDONLY))==-1)  /* 以只读方式打开命名管道 */
 {
  perror("Can't open the FIFO");
  return 1;
 }

 while(1)  /* 不断从管道中读取信息 */
 {
  read( fd, buf, sizeof(buf) );
  printf("Message from Client: %s\n",buf );
  sleep(3); /* sleep 3s */
 }

 close(fd);  /* 关闭管道 */
 return 0;
}


