#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/ioctl.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include<time.h>

#define MAXSIZE 7
#define on 1
#define off 0



void changetochar(unsigned char *PBuff,unsigned char *back)
{
	//printf("enter\n");
	//unsigned char src[22] ;
	unsigned char temp;
	int i;
	back[0] = ' ';
	for(i = 0; i< 7; i++)
	{
		if(((PBuff[i] & 0xf0)>>4) < 10)
			temp =  ((PBuff[i] & 0xf0)>>4) +48;
		else
			temp = 	((PBuff[i] & 0xf0)>>4) +'a' -10;
		back[i*3 + 1]= temp;
		
		
		if((PBuff[i] & 0xf) < 10)
			temp =  (PBuff[i] & 0xf) +48;
		else
			temp = 	(PBuff[i] & 0xf) +'a' -10;
		back[i*3 + 2] = temp;
		
		back[i*3 + 3] = ' ';	
		
	}
	back[22] = '\0';
	
	

}



int wait_for_connection(int port )
{
	int s,rc,newsock,addr_len;
	int optval=1;
	int i;
	struct sockaddr_in my_addr,remote_addr;
	
	
	FILE *fp;
		
	time_t now;						//实例化time_t结构
	struct tm *timenow;	 			//实例化tm结构指针
	unsigned char time_data[80];	
	
	
	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//1
	
	if(s == -1)
	{
		printf("Socket error!");
		return -1;
	}
	
	rc = setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&optval,20);
	
	if(rc !=0)
	{
		printf("set socket opt error");
		return  -1;
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	rc = bind(s,(struct sockaddr *)(&my_addr),sizeof(my_addr));//绑定本地地址	2
	
	if(rc == -1)
	{
		printf("bind socket error");
		return  -1;
	}
	rc = listen(s,10);	//3
	
	if(rc == -1)
	{
		printf("listen error");
		return  -1;
	}
	i =0 ;
	unsigned char PBuff[MAXSIZE];
	unsigned char back[200];
	unsigned int ByteNum;
	unsigned int led_flash;
	
	addr_len =sizeof(remote_addr);
	while(1)
	{
		newsock = accept(s,(struct sockaddr *)&remote_addr,&addr_len);//建立连接	4
			
		if(newsock == -1)
		{
			printf("accept error");
			printf("%s\n",strerror(errno));
			continue;
		}
			
		while(1)		
		{
			ByteNum = recv(newsock,PBuff,MAXSIZE,0);
				
			if(ByteNum < 0)
				continue;				
			else
			{
				{
					printf("recvive:%s\n",PBuff);			
				}
					
			}
		/*	//校验
			if(PBuff[0] == 0x68)
			{	//CS校验
				if(PBuff[0] + PBuff[1] + PBuff[2] + PBuff[3] + PBuff[4] == PBuff[5])
				{
					led_flash = PBuff[4];
					led_flash =	(led_flash << 8) + PBuff[3];
					led_flash = (led_flash << 8) + PBuff[2];
					led_flash = (led_flash << 8) + PBuff[1];			
					printf("led_flash:%x\n",led_flash);
				
				
					changetochar(PBuff,back);
					puts(back);
					//for(i = 0 ;i< 22 ;i++)
						//putchar(back[i]);
					
					time(&now);						//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
					timenow = localtime(&now);	    //localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区
					
					strftime(time_data,sizeof(time_data),"%Y-%m-%d %H:%M:%S",timenow);
					//printf("time_data:%d",strlen(time_data));
					time_data[20] = ' ';
					//日志文件的创建修改
					fp = fopen("output.logo","a+");
					if(fp == NULL)
						exit(0);
					strcat(time_data,back);	
					fwrite(time_data,strlen(time_data),1,fp);
					fprintf(fp,"次数：%x\n",led_flash);
					
					fclose(fp);
					//控制LED灯的闪烁次数
					/*for(i = 0;i < led_flash;i++)
					{
						ioctl(led_fd, on, 1);
						usleep(500000);
						ioctl(led_fd, off, 1);
						usleep(500000);
					}
				}
				else
				{
					printf("CS error\n");
					continue;
				}
			}
			else
			{
				printf("receive data error \n");
				continue;
			}*/
		}
	}	
	close(newsock);
		
	
	close(s);
	return newsock;
}
int main(int argc,char **argv)
{
	

	if(argc != 2)
	{
		printf("please input :name port ");
		exit(0);
	}
	
	/*
	int led_fd;
	led_fd = open("/dev/GPIO-Control",0);
	
	if (led_fd < 0)
	{
		printf("open device leds error\n");
		exit(1);
	}
	else
		printf("open device leds ok\n");
	*/
	
	//ioctl(fd, on, led_no);
	
	u_int32_t tcp_port_no = atoi(argv[1]);

	int ret = wait_for_connection(tcp_port_no);
	if(ret == -1)
	{
		printf("failed to accept connection from peer\n");
		return -1;
	}
}
