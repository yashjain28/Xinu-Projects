/*  main.c  - main */

#include <xinu.h>
#define SIZE 20
pid32 producer_id;
pid32 consumer_id;
pid32 timer_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;

/* Define your circular buffer structure and semaphore variables here */
/* */
sid32 check;
int32 buff[SIZE]={0},c=0,in=0,out=0,empty=SIZE,full=0;

/* Place your code for entering a critical section here */
void mutex_acquire(sid32 mutex)
{
	/* */
	wait(mutex);
}

/* Place your code for leaving a critical section here */
void mutex_release(sid32 mutex)
{
	/* */
	signal(mutex);
}

/* Place the code for the buffer producer here */
process producer(void)
{
	int32 j=1;
	while(1)
        {
		j++;		
		mutex_acquire(check);
                if(empty>0){
			buff[in]=j;
			empty=(empty-1);
			full=(full+1);
			printf("p%d=%d  ",in,j); /*printf is in critical section just for users convenience so that the output is not broken*/
			in=(in+1)%SIZE;		
		}
		mutex_release(check);
        }

	return OK;
}

/* Place the code for the buffer consumer here */
process consumer(void)
{
	/* Every time your consumer consumes another buffer element,
	 * make sure to include the statement:
	 *   consumed_count += 1;
	 * this will allow the timing function to record performance */
	/* */
	int32 temp;
	while(1)
        {	
		mutex_acquire(check);               
		if(full>0){
			full=(full-1);
			empty=(empty+1);
			temp=buff[out];
			buff[out]=-1;
		        printf("c%d=%d ",out,temp);
			out=(out+1)%SIZE;
			consumed_count+=1;		
		}
		mutex_release(check);
        }

	return OK;
}


/* Timing utility function - please ignore */
process time_and_end(void)
{
	int32 times[5];
	int32 i;

	for (i = 0; i < 5; ++i)
	{
		times[i] = clktime_ms;
		yield();
		consumed_count = 0;
		while (consumed_count < CONSUMED_MAX * (i+1))
		{
			yield();
		}

		times[i] = clktime_ms - times[i];
	}

	kill(producer_id);
	kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{
		kprintf("TIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}

process	main(void)
{
	recvclr();

	/* Create the shared circular buffer and semaphores here */
	/* */
	check=semcreate(1);
	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
	timer_id = create(time_and_end, 4096, 50, "timer", 0);

	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	/* Uncomment the following line for part 3 to see timing results */
	resume(timer_id);
	resched_cntl(DEFER_STOP);
	return OK;
}
