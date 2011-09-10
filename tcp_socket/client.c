#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define MAXSIZE		7

int create_tcp_connection(const char *ip,int port)		//ip address port
{
	struct in_addr remote_addr;		//32位IP地址
	int s,rc; struct sockaddr_in my_addr,server_addr;
	int i;
	int optval = 20;
	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //创建套接字AF_INET	PF_INET?  1
	
	if(s == -1)
	{
		printf("error occured when create socket\n");  return -1;
	}
	rc = setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&optval,20);//设置socket选项

	if(rc !=0)
	{
		printf("Set Socket Opt error!");
		return -1;
	}
 
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	
	if(inet_aton(ip,&remote_addr)==0)
	//inet_aton()是一个改进的方法来将一个字符串IP地址转换为一个32位的网络序列IP地址
	{
		printf("IP Address is Wrong!");
		return -1;
	}
 
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = remote_addr;
	unsigned char pMsg[MAXSIZE] = {0x68,0x00,0x00,0x00,0x00,0x00,0x16};
	unsigned int led_flsh;
	int cnt;
	while(1)
	{
		rc = connect(s,(struct sockaddr *)(&server_addr),sizeof(server_addr));	//2
		
		if(rc == -1)
		{
			#ifdef DEBUG
			printf("%s\n",strerror(errno));
			#endif
			printf("Connect error!");
			continue;
		}
		//while(fgets(pMsg,MAXSIZE,stdin) != NULL)
		while(1)
		{					
			printf("please inout  led flash time :	");
			scanf("%d",&led_flsh);
			printf("led_flsh:%d\n",led_flsh);
			//将32位整型数据拆分成4个字节
			pMsg[1] = led_flsh & 0xff;
			pMsg[2] = (led_flsh & 0xff00)>>8;
			pMsg[3] = (led_flsh & 0xff0000)>>16;
			pMsg[4] = (led_flsh & 0xff000000)>>24;
			//CS校验
			pMsg[5] = pMsg[0] + pMsg[1] + pMsg[2] + pMsg[3] + pMsg[4];
			
			for(i = 0;i < 7;i++)
				printf("pMsg%d:0x%x\n",i,pMsg[i]);			
			cnt=send(s,pMsg,sizeof(pMsg),0);//成功返回发送字节数，错误 返回-1
			
			if(cnt==-1)
			{	
				printf("send error \n");
				
			}
		}
	}
	return s;
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("please input :name ip port");
		exit(0);
	}

	u_int32_t tcp_port_no =atoi(argv[2]);
	int ret = create_tcp_connection(argv[1],tcp_port_no);
	if(ret == -1)
	{
		printf("failed to create tcp connection with peer\n");
		return -1;
	}
}
