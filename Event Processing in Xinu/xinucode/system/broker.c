#include <xinu.h>

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
