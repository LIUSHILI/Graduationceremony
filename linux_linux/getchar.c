/***************************************************************
**Function  :Use malloc method input string
**Date	    :2011.11.24
**Author    :LiuShiLi
**
***************************************************************/



#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

void getstring(char *string,char **argv);
const int SIZE = 16;
const int STRNUM = 2;

int main(void)
{
    char *eptr,*sptr,*tptr,ch;
    int cread = 0,csize = 0,ich;
    char **argv;

    argv = malloc(STRNUM);
    csize = SIZE;
    sptr = eptr = malloc(SIZE);
    ich = getchar();
    while(1)
    {
	//ich = getchar();
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

    getstring(sptr,argv);

    free(sptr);
    return 0;
}
void getstring(char *string,char **argv)
{
    int i,len,index = 1;
    int strsize = 0,strread = 1;
    char **tptr,**ptr;
    
    ptr = argv;
    strsize = STRNUM;

    len = strlen(string);
    printf("len :%d\n",len);
    
    argv[0] = string;
    
    for(i = 0;i < len;i++)
	if(string[i] == 0x20)
	{
	    string[i] = '\0';
	    if(string[i+1] != 0x20)
	    {
		strread++;
	/*	if(strread == strsize)
		{
		    printf("strread == strsize\n");
		    tptr = malloc(strsize + STRNUM);
		    memcpy(tptr,ptr,strsize);
		    strsize +=STRNUM;
		    free(ptr);
		    ptr = tptr;
		}
	*/	
		ptr[index++] = &string[i+1];
	    }
	}
    for(i = 0;i < index;i++)
	printf("argv[%d]:\n%s\n",i,ptr[i]);


}
