#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item START_NUMBER, END_NUMBER;

int insert_item(buffer_item item);

int remove_item(buffer_item *item);


buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
    /* Implementation of the insert_item() function */
    //Wait for empty semaphore
	sem_wait(&empty);
	//Get mutex lock to secure buffer
	pthread_mutex_lock(&mutex);
	buffer[insertPointer++] = item;
	insertPointer = insertPointer % BUFFER_SIZE;
	START_NUMBER++;
	//Release mutex and filled semaphore
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	return 0;
}


int remove_item(buffer_item *item)
{
	/* Implementation of the remove_item function */
	//Get filled semaphore
	sem_wait(&full);
	//Get mutex lock to secure buffer
	pthread_mutex_lock(&mutex);
	*item=buffer[removePointer];
	buffer[removePointer++]=-1;
	removePointer=removePointer % BUFFER_SIZE;
	END_NUMBER++;
	//Release mutex lock and empty semaphore
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	return 0;
}

int main(int argc, char *argv[])
{
/* 1. Get command line arguments argv[1],argv[2],argv[3],argv[4] */
/* 2. Initialize buffer */
/* 3. Create producer thread(s) */
/* 4. Create consumer thread(s) */
/* 5. Sleep */
/* 6. Exit */
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != BUFFER_SIZE)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads> <start number>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);
	START_NUMBER = (atoi(argv[4])-1);
	END_NUMBER = (atoi(argv[4])-1);

	/* Initialize the synchronization tools */
	printf("%d\n",pthread_mutex_init(&mutex,NULL));
	printf("%d\n",sem_init(&empty,0,BUFFER_SIZE));
	printf("%d\n",sem_init(&full,0,0));
	srand(time(0));

	/* Create the producer and consumer threads */
	for(i=0;i<producerThreads;i++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid,&attr,producer,NULL);
	}
	for(j=0;j<consumerThreads;j++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid,&attr,consumer,NULL);
	}
	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
	/* Implementation of the producer thread -- refer to Figure 5.26 on page 256 */
	buffer_item Producer;
	int r, id1;
	id1 = pthread_self();
	while(TRUE)
	{	
		r = rand() % BUFFER_SIZE;
		sleep(r);
		Producer = rand();
		//sleep(sleepTime);
		//start = START_NUMBER;
		if(insert_item(Producer)) 
		{
			fprintf(stderr,"Error");
		}
		else
		{
		printf("Producer %u produced %d\n",id1,START_NUMBER);
		}
	}
}

void *consumer(void *param)
{
	/* Implementation of the consumer thread -- refer to Figure 5.26 on page 256 */
	buffer_item Consumer;
	int r, id2;
	id2 = pthread_self();
	while(TRUE)
	{	
		r = rand() % BUFFER_SIZE;
		sleep(r);
		//Consumer = rand();
		//leep(sleepTime);
		//start = START_NUMBER;
		if(remove_item(&Consumer)) 
		{
			fprintf(stderr,"Error Consuming");
		}
		else 
		{
			printf("Consumer %u consumed %d\n",id2,END_NUMBER);
		}
	}
}