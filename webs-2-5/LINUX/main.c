/*
 * main.c -- Main program for the GoAhead WebServer (LINUX version)
 *
 * Copyright (c) GoAhead Software Inc., 1995-2010. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 */

/******************************** Description *********************************/

/*
 *	Main program for the GoAhead WebServer.
 */

/********************************* Includes ***********************************/

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>

#define DEBUG			//Modify by :LuiShiLi
						//Date		:2011.09.29

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif


/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("www");			/* Root web directory */
static char_t		*demoWeb = T("wwwdemo");		/* Root web directory */
static char_t		*password = T("");				/* Security password */
//static int			port = WEBS_DEFAULT_PORT;		/* Server port 8080*/
static int			port = 8888;
static int			retries = 5;					/* Server port retries */
static int			finished = 0;					/* Finished flag */

/****************************** Forward Declarations **************************/

static int 	initWebs(int demo);
static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);
static void	sigintHandler(int);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */

int main(int argc, char** argv)
{
	int i, demo = 0;

	for (i = 1; i < argc; i++) 
	{
		if (strcmp(argv[i], "-demo") == 0) 
		{
			demo++;
		}
	}

	#ifdef DEBUG
		printf("demo:%d\n",demo);		//Modify by :LuiShiLi 
	#endif								//date		:2011.09.28
	

	
	//首先分配一个大的内存块（60*1024字节），以后只要是以b开头的
	//对内存操作的函数都是在这个已经分好的内存块上的操作，
	//这些操作在Balloc.c中实现。

 
	bopen(NULL, (60 * 1024), B_USE_MALLOC);
	signal(SIGPIPE, SIG_IGN);			//管道破裂，写一个没有读端口的管道
	signal(SIGINT, sigintHandler);		//按键中断
	signal(SIGTERM, sigintHandler);		//终止信号


	/*
	
	Initialize the web server
	
	初始化用户管理部分，打开web服务器，注册URL处理函数。
	
	用户管理部分在um.c中实现，
	
	Web服务器的初始化是在default.c和webs.c中实现
	
	url处理函数在handler.c中实现
	
	 */

	if (initWebs(demo) < 0) 
	{
		return -1;
	}

	
#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
/*	websRequireSSL("/"); */	/* Require all files be served via https */
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */

//主循环
	finished = 0;
	while (!finished) 
	{
	
		/*
		
		1，socketReady()函数检查是否有准备好的sock事件
		
		2，socketSelect()函数首先把各个sock感兴趣的事件 （sp->handlerMask）
		注册给三个集合（读，写，例外），然后调用select系统调用，然后更新各个sock
		的 sp->currentEvents，表示各个sock的当前状态。
		
		这两个函数在sockGen.c中实现，他们主要操作的数据是socket_t变量 socketList中
		的handlerMask和currentEvents，socketList在sock.c中定义并主要由该文件中的 
		socketAlloc，socketFree和socketPtr三个函数维护。
		
		*/
		if (socketReady(-1) || socketSelect(-1, 1000)) 
		{
			
			/*
			
			该函数处理具体的sock事件
			
			1，调用socketReady(sid)对socketList[sid]进行检查，看是否有sock事件
			
			2，如果有sock事件，则调用socketDoEvent()函数，对事件进行处理
			
			*/
			socketProcess(-1);
		}

		
		/*
		
		该函数在cgi.c中实现，检查cgiRec变量cgilist，首先把cgi的结果输出，如果有的话，然后看cgi进程是否已对号束，如果结束，就清理该cgi进程。
		
		Cgilist在函数websCgiHandler和websCgiCleanup中维护。




		
		*/
		websCgiCleanup();
		
		/*
		
		该函数在websuemf.c中实现，功能是检查sched_t变量sched，断开超时的连接，sched变量在emfSchedCallback和emfUnschedCallback中维护
		
		*/
		emfSchedProcess();
	}

	
	/*
	
	退出时的清理工作，永远不会执行到这里
	
	*/

#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	bclose();
	return 0;
}

/*
 *	Exit cleanly on interrupt
 */
static void sigintHandler(int unused)
{
	finished = 1;
}

/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs(int demo)
{
	struct hostent	*hp;
	struct in_addr	intaddr;
	char			host[128], dir[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];

	/*
	 *	Initialize the socket subsystem
	 */
	 
	socketOpen();

	#ifdef USER_MANAGEMENT_SUPPORT
		/*
		 *	Initialize the User Management database
		 */
		umOpen();
		umRestore(T("umconfig.txt"));
	#endif

	/*
	 *	Define the local Ip address, host name, default home page and the 
	 *	root web directory.
	 */


												
	if (gethostname(host, sizeof(host)) < 0) 
	{
		error(E_L, E_LOG, T("Can't get hostname"));			//Modify by :LuiShiLi
		return -1;											//Date		:2011.09.28
	}
	if ((hp = gethostbyname(host)) == NULL) 
	{
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}
	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
	
	(size_t) hp->h_length);
	
	//intaddr.s_addr = inet_addr("10.0.0.9");



	/*
	 *	Set ../web as the root web. Modify this to suit your needs
	 *	A "-demo" option to the command line will set a webdemo root
	 */
	getcwd(dir, sizeof(dir)); 		//取得当前的工作目录:/home/forlinuxsmb/../


	
	if ((cp = strrchr(dir, '/')))	//寻找出'/'最后出现的位置
	{
		*cp = '\0';
	}
	
	#ifdef DEBUG										//Modify by :LuiShiLi
		printf("Working directory is :%s\n",dir);		//Date		:2011.09.28
	#endif
	
	if (demo) 
	{
		sprintf(webdir, "%s/%s", dir, demoWeb);
	}
	else
	{
		sprintf(webdir, "%s/%s", dir, rootWeb);
	}
	
	#ifdef DEBUG
		printf("Web directory is :%s\n",webdir);		//Modify by :LuiShiLi
	#endif												//Date		:2011.09.28
	

	/*
	 *	Configure the web server options before opening the web server
	 */
	websSetDefaultDir(webdir);
	
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
	
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
	websSetHost(wbuf);

	/*
	 *	Configure the web server options before opening the web server
	 */
	websSetDefaultPage(T("default.asp"));

	#ifdef DEBUG
		printf("T\(\"default.asp\"\) is :%s\n",T("default.asp"));
	#endif
	
	websSetPassword(password);

	/* 
	 *	Open the web server on the given port. If that port is taken, try
	 *	the next sequential port for up to "retries" attempts.
	 */
	websOpenServer(port, retries);

	

	/*
	 * 	First create the URL handlers. Note: handlers are called in sorted order
	 *	with the longest path handler examined first. Here we define the security 
	 *	handler, forms handler and the default web page handler.
	 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, 
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, 
		WEBS_HANDLER_LAST); 

/*
 *	Now define two test procedures. Replace these with your application
 *	relevant ASP script procedures and form functions.
 */
	websAspDefine(T("aspTest"), aspTest);		//关联ASP标签和C函数名字

	websFormDefine(T("formTest"), formTest);	//关联GoForms标签和C函数名字

/*
 *	Create the Form handlers for the User Management pages
 */
#ifdef USER_MANAGEMENT_SUPPORT
	formDefineUserMgmt();
#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0); 
	return 0;
}

/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to 
 *	"localhost/asp.asp" to test.
 */

static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;

	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2)
	{
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	return websWrite(wp, T("Name: %s, Address %s"), name, address);
}

/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
 */

static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith")); 
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave.")); 

	websHeader(wp);
	//websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websWrite(wp, T("<body><h2>result:%d\n</h2>\n"), name); 	//Modify By LiuShiLi 2011.10.11
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		websRedirect(wp, WEBS_DEFAULT_HOME);
		return 1;
	}
	return 0;
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks() 
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY, 0666)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

/******************************************************************************/
