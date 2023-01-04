#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


int fib[1000];
void *fibonnaci(void *param);
int userIn;
int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	fib[0] = 0;
	fib[1] = 1;
	printf("Enter number of Fibonacci numbers to generate: ");
	scanf("%d",&userIn);
	pthread_create(&tid,&attr,fibonnaci,NULL);
	pthread_join(tid,NULL);
	for(int i = 0;i<userIn;i++)
	{
		printf("Fibonnaci number %d: %d\n",i+1,fib[i]);
	}
	return 0;
}

void *fibonnaci(void *param)
{

        for (int i = 2;i<userIn;i++)
        {
                fib[i] = fib[i-1]+fib[i-2];
        }
pthread_exit(0);
}
