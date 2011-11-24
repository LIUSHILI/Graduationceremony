#include    <stdio.h>
#include    <limits.h>
	
	//thebigger,thefaster
	//butitmaywastemorememory
	//***itmustbepositive
const int SIZE_INC=16;
	
int main()
{
	char *astr,*tptr,*tpt,ch;
	int cread=0,csize=0,ich;
	//read
	tpt=astr=malloc(SIZE_INC);
	
	csize=SIZE_INC;
	
	ich=getchar();
	
	while(1)
	{
	    if(ich=='\n'||ich==EOF)
		ch='\0';
	    else
		ch=ich;
	    if(cread==csize)
	    {
		tptr=malloc(csize+SIZE_INC);
		memcpy(tptr,astr,csize);
		free(astr);
		csize+=SIZE_INC;
		astr=tptr;
		tpt=astr+cread;
	    }
	    *tpt=ch;
	    if(ch=='\0')
		break;
	    cread++;
	    tpt++;
	    ich=getchar();
	}
	//write
	printf("%d char(s) are read intotal,they are:\n%s",cread,astr);
	//remembertoreleaseusedmemory
	free(astr);
	//wait
	getchar();

	return 0;
}
