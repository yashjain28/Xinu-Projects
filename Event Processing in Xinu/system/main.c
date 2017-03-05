/*  main.c  - main */

#include <xinu.h>

pid32 publisher1;
pid32 subs1;
pid32 unsubs1;
pid32 publisher2;
pid32 subs2;
pid32 broker1;

void handler1(topic16 t,uint32 mesg){
		kprintf("\tmesg %d received 4m group %d topic %d \t",(mesg),(t >> 8),(t & 255));
}

void handler2(topic16 t,uint32 mesg){
		kprintf("\tmesg %d received 4m group %d topic %d \t",(mesg),(t >> 8),(t & 255));
}

void handler3(topic16 t,uint32 mesg){
		kprintf("\tmesg %d received 4m group %d topic %d \t",(mesg),(t >> 8),(t & 255));
}


process Publish(void)
{
	//kprintf("publishing now\n")
	/*	publish(5,10);
		publish(10,11);
		publish(12,12);
		publish(12,13);*/
		publish(10,14);
		publish(5,15);
		publish(300,16);
		publish(5,100);
		//publish(44,17);
		publish(261,18);
		publish(301,19);
		publish(299,9);

	//kprintf("donePublishing\n");

	return OK;
}

process Publish1(void)
{
	//kprintf("publishing now\n")
		publish(5,20);
		publish(10,21);
		publish(12,22);
		publish(305,23);
		
	//kprintf("donePublishing\n");

	return OK;
}
/*
process Broker(void){
	topic16 topic;
	uint32 mesg;
	int32 x;
	kprintf("entering broker\n");
	while(1){
			if(headbroker!=NULL){
				//kprintf("hii\t");
				//x = printblist();
				head1 = headbroker;
				topic = head1->topicID;
				//kprintf("topicBB %d",topic);
				mesg  = head1->data;
				//procID= head1->procid;
				x = delnodeblist();
				x = handlerprint(topic,mesg);
			}
			
	}		
	kprintf("outofbroker\n");
	return OK;
}
*/
process subscriber1(void){
		
		subscribe(44,handler1);
		subscribe(45,handler1);
		subscribe(261,handler3);
		//subscribe(10,handler1);
		//kprintf("\tsub %d\t",*p);
		//subscribe(12,handler2);
		//kprintf("\tsub %d\t",*p);
		//unsubscribe(10);
		//subscribe(5,handler3);
		//kprintf("\tsub %d\t",*p);
		//unsubscribe(5);
		subscribe(5,handler1);

		return OK;
}
process subscriber2(void){

		subscribe(300,handler3);
		subscribe(5,handler2);
		subscribe(261,handler1);
		//subscribe(10,handler1);
		//kprintf("\tsub %d\t",*p);
		//subscribe(12,handler2);
		//kprintf("\tsub %d\t",*p);
		//subscribe(5,handler3);
		//kprintf("\tsub %d\t",*p);
		//subscribe(14,handler3);
		return OK;
}
process unsubsribe1(void){
	unsubscribe(10);
	//subscribe(10,handler1);
	subscribe(5,handler3);
	//subscribe(14,handler2);
	unsubscribe(10);
}
process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	publisher1=create(Publish, 4096, 50, "Pproc 1",0);
	publisher2=create(Publish1, 4096, 50, "Pproc 2",0);
	subs1=create(subscriber1, 4096, 50, "Sproc1",0);
	subs2=create(subscriber2, 4096, 50, "Sproc2",0);
	unsubs1=create(unsubsribe1, 4096, 50, "Uproc1",0);
	broker1=create(Broker, 4096, 50, "Broker",0);	
	
	resched_cntl(DEFER_START);
	resume(subs1);
	resume(subs2);
	
	resume(publisher1);
	resume(unsubs1);
	resume(publisher2);

	resume(broker1);
	resched_cntl(DEFER_STOP);
	
	kprintf("\nending my main code\n");
	return OK;
    
}
