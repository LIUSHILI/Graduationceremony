#include "stdio.h"

void QuickSort(int e[], int first, int end)
{
	int i=first,j=end,temp=e[first];
	while(i<j)
  {
		while(i<j && e[j]>=temp)
			j--;
		e[i]=e[j];
		while(i<j && e[i]<=temp)
			i++;		
		e[j]=e[i];
  }
	e[i]=temp;
  if(first<i-1)
  	QuickSort(e,first,i-1);
	if(end>i+1)		
		QuickSort(e,i+1,end);
}

void main()
{
	int arr[] = {49, 38, 65, 97, 76, 13, 27, 49};
	int len = 8;
	int i;
	printf("before sort\n");
	for(i=0; i<len; i++)
		printf("%d  ", arr[i]);
	printf("\n");
	
	QuickSort(arr, 0, len-1);
	
	printf("after sorted\n");
	for(i=0; i<len; i++)
		printf("%d  ", arr[i]);
	printf("\n");
}