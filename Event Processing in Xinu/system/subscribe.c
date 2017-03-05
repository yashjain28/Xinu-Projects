
#include <xinu.h>


syscall  subscribe(topic16  topic,  void  (*handler)(topic16,  uint32)){

		int mask;
		int i=0;
		int count=0;
    	struct	topicent *procfuncptr;
    	//struct  procent *prptr;
    	mask=disable();
    	if(isbadtopic(topic & 255)){
    		restore(mask);
    		return SYSERR;
    	}
		procfuncptr=&topictab[(topic & 255)]; 
		i=0;   	
		count=0;
		while((procfuncptr->substopic[i].procID!=-1) && (procfuncptr->substopic[i].procID!=currpid) && count<8){
			count++;
			i++;
		}
		if(procfuncptr->substopic[i].procID==currpid){
			kprintf("Proc cannot subscribe to same topic\n");
			restore(mask);
			return OK;
		}
		if(count<8){
				procfuncptr->substopic[i].procID=currpid;
				procfuncptr->substopic[i].handle=handler;
				procfuncptr->substopic[i].groupID=(topic >> 8);
				
				kprintf("Proc %d subs to topic %d in group %d \n",currpid,(topic & 255),(topic >> 8));
		}
		else{
			kprintf("LimitReached for topic %d\n",(topic & 255));
		}

			restore(mask);
			return OK;
		}





	
  