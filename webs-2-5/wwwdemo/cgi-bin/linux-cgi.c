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
   printf("<TITLE> �˷���� </TITLE>\n");
   printf("</head>\n");
   
   printf("<body><br>\n");
   printf("<H3>�˷����</H3>");
   data = getenv("QUERY_STRING");
   if(data == NULL) 
      printf("<P>��������û�б�����������ݴ���������\n"); 
   else
   {	//for(i=0;i++;i<32)
	    	printf("<P>Data is :%s\n",data);
		sscanf(data,"m=%ld&n=%ld",&m,&n);
		printf("<P>%ld x %ld �Ľ���ǣ�%ld ",m,n,m*n);
	  /*printf("<P>%ld x %ld �Ľ���ǣ�%ld ",m,n,m*n);
      if(sscanf(data,"m=%ld&n=%ld",&m,&n)!=2) 
          printf("<P>�����������ݷǷ�����������ı��������֡�"); 
      else 
          printf("<P>%ld x %ld �Ľ���ǣ�%ld ",m,n,m*n); 
		  */
		  
   } 
   printf("\n"); 
   printf("</body>\n");
   printf("</html>\n");
   return (0); 
}
