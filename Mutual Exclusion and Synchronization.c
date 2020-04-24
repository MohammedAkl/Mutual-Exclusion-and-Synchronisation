#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include<time.h>
#include<semaphore.h>
#define sizeB 5
int r,count ,temp , temp2 ,n1, b1=0,b2=0, i=1;
int buffer[sizeB];
sem_t sem;
sem_t Csem;
sem_t s;
sem_t e;
sem_t n;
void randomNum(){
	int upper=4;
	int lower=1;
	r=(rand() % (upper-lower+1))+lower;
}

	
void *counter(void *arg){
	sem_wait(&Csem);
	int itemp = i;
	i++;
	sem_post(&Csem);
while(1){
	
	srand(time(0));
	randomNum();
	sleep(r);
	printf("Counter thread%d : received a message \n",itemp);
	int x =sem_trywait(&sem);
	if(x!=0){
		printf("++++Counter thread%d : waiting to write\n",itemp);
		sem_wait(&sem);
		}
	count++;
	printf("Counter thread%d : now adding to counter , counter value = %d\n",itemp,count);
	sem_post(&sem);
	
	}
}

void *monitor(void *arg){
while(1){
	srand(time(0));
	randomNum();
	sleep(r);
	printf("Monitor thread : waiting to read counter\n");
	sem_wait(&sem);
	temp=count;
	count=0;
	printf("Monitor thread : reading a count value of %d\n",temp);
	sem_post(&sem);
	//producer
	int x =sem_trywait(&e);
	if(x!=0){
		printf("/////////Monitor thread : Buffer Full !!\n");	
		sem_wait(&e);
		}
	sem_wait(&s);
	buffer[b1]=temp;
	printf("Monitor thread : writing to buffer at postion %d\n" ,b1);
	b1++;
	if (b1==sizeB)
		b1=0;
	sem_post(&s);
	sem_post(&n);
	
	}
}

void *collector(void *arg){
while(1){
	srand(time(0));
	randomNum();
	sleep(r);
	//consumer
	int x =sem_trywait(&n);
	if(x!=0){
		printf("***Collector thread :  nothing is in the buffer !!\n");
		sem_wait(&n);
		}	
	sem_wait(&s);
	temp2=buffer[b2];
	printf("Collector thread :  reading from buffer at postion %d \n",b2);
	b2++;
	if (b2==sizeB)
		b2=0;
	sem_post(&s);
	sem_post(&e);
	

	}
}

void main(){
	sem_init(&sem,0,1);
	sem_init(&Csem,0,1);
	sem_init(&s,0,1);
	sem_init(&n,0,0);
	sem_init(&e,0,sizeB);
	printf("Enter numbers of counters : "); 
	scanf("%d",&n1);
	pthread_t threadCounter[n1],threadMonior,threadCollector;
	for( int v = 0;v<n1;v++){
		pthread_create(&threadCounter[v], NULL, counter,NULL);
		}
	pthread_create(&threadMonior, NULL, monitor, NULL);
	pthread_create(&threadCollector, NULL, collector, NULL);
	for(int j = 0;j<n1;j++){
		pthread_join(threadCounter[j],NULL);
		}
	pthread_join(threadMonior,NULL);
	pthread_join(threadCollector,NULL);
	sem_destroy(&sem);
	
}


