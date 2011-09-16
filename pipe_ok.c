#include<stdio.h>
#include<unistd.h>
#include<string.h>
 
#define MAX_LINE 80
void main()
{
	/*创建一个匿名管道*/
	int thePipe[2], ret;
	char buf[MAX_LINE + 1];
	char testbuf[MAX_LINE + 1];
	
	
	if(pipe(thePipe) == 0)
	{
		/*管道创建成功，数组第一个元素中的文件描述符供应用程序读取，数组第二个元素中的文件描述符供应用程序写入*/
		if(fork() == 0)
		{
			/*进程创建成功，子进程*/
			/*关闭写端，从管道读端读取数据并放入缓冲区*/
			
			while(1)
			{
			//close(thePipe[1]);
			ret = read(thePipe[0], buf, MAX_LINE);
			buf[ret] = 0;
			/*输出缓冲区数据，关闭读端，退出*/
			printf("Read :%s\n", buf);
		
			}
		}
		else
		{
			/*进程创建成功*/
			/*关闭读端，向管道写端写入数据*/
			printf("please input the message:");
			while(fgets(testbuf,MAX_LINE,stdin) != NULL)
			{
				printf("write:%s\n",testbuf);
				
				ret = write(thePipe[1], testbuf, strlen(testbuf));
			
				sleep(1);
				printf("please input the message:");
				
			}
		} 
	}
}
