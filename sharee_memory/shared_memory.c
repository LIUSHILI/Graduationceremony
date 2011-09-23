#include<sys/types.h>
#include<linux/sem.h>
#include<linux/shm.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<time.h>

#define MAXSHM 5  //���建����������±��������


/*        ����3���ź������ڲ���ʶ  */

int fullid;
int emptyid;
int mutexid;

/* ������  */
int main()
{
	/*  ����2�������ڴ��ID  */
	int arrayid;
	int getid;
	
	/*  ���干���ڴ������ַ  */
	int *array;
	int *get;
	
	/* ���������ڴ�  */
	arrayid=shmget(IPC_PRIVATE,sizeof(int)*MAXSHM,IPC_CREAT|0666);
	getid=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);
	
	/*  ��ʼ�������ڴ�  */
	array=(int *) shmat(arrayid,0,0);
	get=(int *) shmat(getid,0,0);
	*get=0;
	
	/* �����ź������ݽṹ */
	struct sembuf  P,V;
	union semun arg;
	
	/* �����ź���  */
	fullid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	
	/*��ʼ���ź��� */
	arg.val=0;            //��ʼʱ��������������
	if(semctl(fullid,0,SETVAL,arg)==-1)
	printf("semctl setval error");
	
	arg.val=MAXSHM;       //��ʼʱ����������5�����е�����Ԫ��
	if(semctl(emptyid,0,SETVAL,arg)==-1)
	printf("semctl setval error");
	
	arg.val=1;            //��ʼʱ�����ź�Ϊ1������һ�����̽���
	if(semctl(mutexid,0,SETVAL,arg)==-1)
	printf("semctl setval error");

	/* ��ʼ�� P  V����  */
	P.sem_num=0;
	P.sem_op=-1;
	P.sem_flg=SEM_UNDO;
	V.sem_num=0;
	V.sem_op=1;
	V.sem_flg=SEM_UNDO;

	/*   �����߽���  */
	if(fork()==0)
	{
		int i=0;
		int set=0;
		while(i<10)
		{
			semop(emptyid,&P,1);         //�� emptyidִ��P����
			semop(mutexid,&P,1);         //�� mutexidִ�� P����
			array[set%MAXSHM]=i+1;
			printf("Producer put number %d to No.%d\n",array[set%MAXSHM],set%MAXSHM);
			set++;                       //д������1
			semop(mutexid,&V,1);         //��mutexidִ�� V ����
			semop(fullid,&V,1);          //��fullidִ�� V ����
			i++;
		}
		sleep(3);                    //SLEEP 3�룬�ȴ������߽���ִ�����
		printf("Poducer if over\n");
		exit(0);
	}
	else
	{
	/*  ������A����  */
		if(fork()==0)
		{
			while(1)
			{
				if(*get==10)
				break;
				semop(fullid,&P,1);        //��fullidִ�� P ����
				semop(mutexid,&P,1);       //��mutexidִ�� P ����
				printf("The ConsumerA get number from No.%d\n",(*get)%MAXSHM);
				(*get)++;                   //��������1
				semop(mutexid,&V,1);        //��mutexidִ�� V ����
				semop(emptyid,&V,1);        //��fullidִ�� V ����
				sleep(1);
			}
			printf("ConsunerA is over\n");
			exit(0);
		}
		else
		{
		/*������B����  */
			if(fork()==0)
			{
				while(1)
				{
					if(*get==10)
					break;
					semop(fullid,&P,1);       //��fullidִ�� P ����
					semop(mutexid,&P,1);      //��mutexidִ�� P ����
					printf("The ConsumerA get number from No.%d\n",(*get)%MAXSHM);
					(*get)++;                 //��������1
					semop(mutexid,&V,1);      //��mutexidִ�� V ����
					semop(emptyid,&V,1);      //��emptyidִ�� V ����
					sleep(1);
				}
				printf("ConsunerB is over\n");
				exit(0);
			}
		}
	}

	/*   �����̷��غ����3���ӽ���  */
	wait(0);
	wait(0);
	wait(0);

	/*  �Ͽ�������2�������ڴ�  */
	shmdt(array);
	shmctl(arrayid,IPC_RMID,0);
	shmctl(get);
	shmctl(getid,IPC_RMID,0);
	/*   ����3���ź�����  */
	semctl(emptyid,IPC_RMID,0);
	semctl(fullid,IPC_RMID,0);
	semctl(mutexid,IPC_RMID,0);
	exit(0);
}

