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
#define COMMANDSIZE	10

#define DEBUG

const int SIZE = 16;
const int STRNUM = 2;


int creat_tcp_connection(const char *ip,int port);

int getstring(void); //get a string command

char **geteachstring(char *string,char **argv);	//egt each parameter

/***************************Function :getstring()*********************************/

int getstring(void)
{
    char *eptr,*sptr,*tptr,ch;
    int cread = 0,csize = 0,ich;
    char **argv;

    argv = malloc(STRNUM*sizeof(char *));
    csize = SIZE;
    sptr = eptr = malloc(SIZE);
    ich = getchar();
    while(1)
    {
	if(ich == '\n' || ich == EOF)
       	    ch = '\0';
	else
	ch = ich;
	
	if(cread == csize)
	{
	    tptr = malloc(csize+SIZE);
	    memcpy(tptr,sptr,csize);
	    free(sptr);
	    csize +=SIZE; 
	    sptr = tptr;
	    eptr = sptr + cread;

	}
	*eptr = ch;
	if(ch == '\0')
	    break;
	cread++;
	eptr++;
	ich = getchar();
    }
    printf("string is:\n%s\n",sptr);

    geteachstring(sptr,argv);

    free(sptr);
    return 0;
}

/*******************Function :geteachstring()****************************************/

char **geteachstring(char *string,char **argv)
{
    int i,j,len,index = 0;
    int strsize = 0,strread = 1;
    char **tptr,**ptr;
    int onestrlen = 0;

    ptr = argv;
    strsize = STRNUM;

    len = strlen(string);
    printf("len :%d\n",len);
    
    ptr[0] = string;
    
    for(i = 0;i < len;i++)
	if(string[i] == 0x20)
	{
	    string[i] = '\0';
	    if(string[i+1] != 0x20)
	    {
		
		ptr[++index] = &string[i+1];
		strread++;
	    
		if(strread == strsize)
		{

		    tptr =(char **)malloc((strsize + STRNUM)*sizeof(char *));
		    
		    for(j = 0;j < strread;j++)
		    {	
			tptr[j] = ptr[j];
		    }

		    strsize +=STRNUM;
		    free(ptr);
		    ptr = tptr;
		}
		
	
	    }
	}
    
    for(i = 0;i <= index;i++)
	printf("ptr[%d]:\n%s\n",i,ptr[i]);

    return ptr;


}

/*******************Function :create_tcp_connection()************************************/

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
	
	char command[COMMANDSIZE];
	
	unsigned char pMsg[] = {0x00,0x00,0x00,0x00,0x00};	//12bytes
	unsigned int led_num;
	int cnt;
	while(1)
	{
		rc = connect(s,(struct sockaddr *)(&server_addr),sizeof(server_addr));	//2
		
		if(rc == -1)
		{
		    printf("Connect error!");
			continue;
		}
		//while(fgets(pMsg,MAXSIZE,stdin) != NULL)
		while(1)
		{	
			printf("please input your command:");


			getstring();

			/*	scanf("%s",command);
			send(s,command,strlen(command),0);
			if(strcmp(command,"end") == 0)
			    break;
			else if(strcmp(command,"start") == 0)
			{   
			    printf("please inout  led number :");
			    scanf("%d",&led_num);
			    printf("led_num:%d\n",led_num);
			    //将32位整型数据拆分成4个字节
			    pMsg[0] = led_num & 0xff;
			    pMsg[1] = (led_num & 0xff00)>>8;
			    pMsg[2] = (led_num & 0xff0000)>>16;
			    pMsg[3] = (led_num & 0xff000000)>>24;
			    //CS校验
			    // pMsg[9] = pMsg[0] + pMsg[1] + pMsg[2] + pMsg[3] + pMsg[4];
			
			    //for(i = 0;i < 7;i++)
			    //printf("pMsg%d:0x%x\n",i,pMsg[i]);			
			
			    cnt=send(s,pMsg,sizeof(pMsg),0);//成功返回发送字节数，错误 返回-1
			
			    sleep(5);
			    if(cnt==-1)
			    {	
				printf("send error \n");
				
			   }
			}
			else
			printf("command error for avaible command input ? for help\n");
		    */
		}
	    break;
	    }
	return 1;
}

/************************************Function :main()*********************************************/

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("Please Input :Name Ip Port\n");
		exit(0);
	}

	u_int32_t tcp_port_no =atoi(argv[2]);	//Port Number
	int ret = create_tcp_connection(argv[1],tcp_port_no);
	if(ret == -1)
	{
		printf("failed to create tcp connection\n");
		return -1;
	}
}
