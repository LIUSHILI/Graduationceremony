#include <stdio.h>
#include <Winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
void main()
{    
	WORD wVersionRequested;   
	WSADATA wsaData;
	SOCKET sockClient;
	SOCKADDR_IN addrSrv; 
	char recvBuf[50]; 

	//WSAata�����洢ϵͳ���صĹ���WinSocket�����ϡ�   
	int err;        
	wVersionRequested = MAKEWORD( 1, 1 );      
	err = WSAStartup( wVersionRequested, &wsaData );   
	if ( err != 0 ) 
	{       
		return; 
	}      
	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 )   
	{       
		WSACleanup( );   
		return;  
	}    
	sockClient=socket(AF_INET,SOCK_STREAM,0);
	// AF_INET ..tcp����    //��ʼ��������˿ں�   

	addrSrv.sin_addr.S_un.S_addr=inet_addr("172.28.30.72");
	//������ַ���������ڱ�������  
	addrSrv.sin_family=AF_INET;    
	addrSrv.sin_port=htons(6000);// ���ö˿ں�    
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//���ӷ����� 
	
//	recv(sockClient,recvBuf,50,0);//��������   
//	printf("%s\n",recvBuf);  
	while(1)
	{
		send(sockClient,"12",strlen("12")+1,0);//��������
		Sleep(1000);
		send(sockClient,"abcde",strlen("abcde")+1,0);//��������
		Sleep(1000);
    }
	closesocket(sockClient);//�ر�����   
	WSACleanup();
} 