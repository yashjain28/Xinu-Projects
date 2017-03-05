
#include <xinu.h>


syscall  unsubscribe(topic16  topic){

		intmask mask;
		int i=0;
		int count=0;
    	struct	topicent *procfuncptr;
    	pid32 tempproc=-1;
    	//struct procent *prptr;
    	topic16 temptopic=-1;
    	int16 tempgroup=0;
    	void (*tempaddr)(topic16,uint32)=NULL;
    	mask=disable();
    	if(isbadtopic(topic & 255)){
    		restore(mask);
    		return SYSERR;
    	}
    	procfuncptr=&topictab[(topic & 255)]; 
		i=0;   	
		while((procfuncptr->substopic[i].procID!=-1) && count<8){
			count++;
			i++;
		}
		if(i>0){
			if(procfuncptr->substopic[i-1].procID!=currpid){
				tempproc=procfuncptr->substopic[i-1].procID;
				tempaddr=procfuncptr->substopic[i-1].handle;
				tempgroup=procfuncptr->substopic[i-1].groupID;
				
			}
		}
		i=0;
		count=0;
		while((procfuncptr->substopic[i].procID!=currpid) && count<8){
			count++;
			i++;
		}	
		if(count!=8 && procfuncptr->substopic[i].procID==currpid){
			procfuncptr->substopic[i].procID=tempproc;
			procfuncptr->substopic[i].handle=tempaddr;
			procfuncptr->substopic[i].groupID=tempgroup;
			
			kprintf("Proc %d: unsubscribed from group %d topic %d\n",currpid,(topic >> 8),(topic & 255));
		/*	prptr=&proctab[currpid];
			i=0;
			count=0;
			while(prptr->prtopicsub[i]!=-1){
				i++;
				count++;
			}
			if(i>0){
					if(prptr->prtopicsub[i-1]!=topic){
						temptopic=prptr->prtopicsub[i-1];
					}
			}
			i=0;
			count=0;
			while(prptr->prtopicsub[i]!=topic){
				i++;
				count++;
			}
			if(count<8 && prptr->prtopicsub[i]==topic){
				prptr->prtopicsub[i]=temptopic;
			}												*/			
		}
		else{
			kprintf("Proc %d never subsrcibed topic %d\n",currpid,topic);
		}


			restore(mask);
			return OK;
}





	
  