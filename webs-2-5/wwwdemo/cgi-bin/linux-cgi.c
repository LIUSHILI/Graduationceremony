#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

int main(void) 
{
   char *data; 
   long m,n;
	int i=0;
   printf("Content-Type:text/html;charset=gb2312 \n\n");
  
   printf("<html>\n");

   printf("<head>\n");
   printf("<TITLE> 乘法结果 </TITLE>\n");
   printf("</head>\n");
   
   printf("<body><br>\n");
   printf("<H3>乘法结果</H3>");
   data = getenv("QUERY_STRING");
   if(data == NULL) 
      printf("<P>错误！数据没有被输入或者数据传输有问题\n"); 
   else
   {	//for(i=0;i++;i<32)
	    	printf("<P>Data is :%s\n",data);
		sscanf(data,"m=%ld&n=%ld",&m,&n);
		printf("<P>%ld x %ld 的结果是：%ld ",m,n,m*n);
	  /*printf("<P>%ld x %ld 的结果是：%ld ",m,n,m*n);
      if(sscanf(data,"m=%ld&n=%ld",&m,&n)!=2) 
          printf("<P>错误！输入数据非法。表单中输入的必须是数字。"); 
      else 
          printf("<P>%ld x %ld 的结果是：%ld ",m,n,m*n); 
		  */
		  
   } 
   printf("\n"); 
   printf("</body>\n");
   printf("</html>\n");
   return (0); 
}
