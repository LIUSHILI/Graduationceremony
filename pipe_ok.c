#include<stdio.h>
#include<unistd.h>
#include<string.h>
 
#define MAX_LINE 80
void main()
{
	/*����һ�������ܵ�*/
	int thePipe[2], ret;
	char buf[MAX_LINE + 1];
	char testbuf[MAX_LINE + 1];
	
	
	if(pipe(thePipe) == 0)
	{
		/*�ܵ������ɹ��������һ��Ԫ���е��ļ���������Ӧ�ó����ȡ������ڶ���Ԫ���е��ļ���������Ӧ�ó���д��*/
		if(fork() == 0)
		{
			/*���̴����ɹ����ӽ���*/
			/*�ر�д�ˣ��ӹܵ����˶�ȡ���ݲ����뻺����*/
			
			while(1)
			{
			//close(thePipe[1]);
			ret = read(thePipe[0], buf, MAX_LINE);
			buf[ret] = 0;
			/*������������ݣ��رն��ˣ��˳�*/
			printf("Read :%s\n", buf);
		
			}
		}
		else
		{
			/*���̴����ɹ�*/
			/*�رն��ˣ���ܵ�д��д������*/
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
