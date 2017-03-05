/* publish.c - publish() */

#include <xinu.h>

syscall  publish(topic16  topic,  uint32  data){

	intmask 	mask;    	/* stores new process id	*/
	struct	topicent	*procfuncptr;		/* pointer to proc. table entry */
	int i=0,count=0;

	mask=disable();
	if (isbadtopic((topic & 255))) {
		restore(mask);
		return SYSERR;
	}
	procfuncptr=&topictab[(topic & 255)];
	if(procfuncptr->substopic[0].procID==-1){
		kprintf("\tNo process Subscribed on topic %d \n",topic);
		restore(mask);
		return SYSERR;
	}
	else if(procfuncptr->substopic[0].procID!=-1){
		i=0;
		count=0;
		while(i<8){
			if(procfuncptr->substopic[i].procID!=-1 && (procfuncptr->substopic[i].groupID==(topic >> 8) || (topic >> 8)==0))
			{
				count=1;
				break;
			}
			i++;
		}
		if(count==1){
			addnodeblist(topic,data);
			kprintf("\tproc %d published data: %d to topic %d in group %d \n",currpid,data,(topic & 255),(topic >> 8 ));
		}
	}
		restore(mask);
		return OK;
	
}