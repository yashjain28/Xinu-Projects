#include <xinu.h>

syscall unsubkill(pid32 prociD){
		intmask mask;
		int i=0,k=0;
		int count1=0,count=0;
    	struct	topicent *procfuncptr;
    	pid32 tempproc=-1;
    	topic16 temptopic=-1,topic;
    	int16 tempgroup=0;
    	void (*tempaddr)(topic16,uint32)=NULL;
    	mask=disable();
    	kprintf("inside unsubkill\t");
    	if(isbadpid(prociD)){
    		restore(mask);
    		return SYSERR;
    	}
    	//kprintf("not bad pid\t");
    	while(k< NTOPIC){
    		//kprintf("t=%d",k);
    		procfuncptr=&topictab[k];
    		topic=k;
			count1=0;
			tempproc=-1;
			tempgroup=0;
			tempaddr=NULL;
			//kprintf("fine %d\t",count1);
						if(procfuncptr->substopic[count1].procID!=-1){
									
    								//kprintf("inside if %d\t",count1);
									i=0;   	
									count=0;
									while((procfuncptr->substopic[i].procID!=-1) && count<8){
										count++;
										i++;
									}
									if(i>0){
										if(procfuncptr->substopic[i-1].procID!=prociD){
											tempproc=procfuncptr->substopic[i-1].procID;
											tempaddr=procfuncptr->substopic[i-1].handle;
											tempgroup=procfuncptr->substopic[i-1].groupID;
											
										}
									}
									i=0;
									count=0;
									while((procfuncptr->substopic[i].procID!=prociD) && count<8){
										count++;
										i++;
									}	
									if(count!=8 && procfuncptr->substopic[i].procID==prociD){
										kprintf("Proc %d: unsubscribed from group %d topic %d\n",prociD,procfuncptr->substopic[i].groupID,(topic & 255));
										procfuncptr->substopic[i].procID=tempproc;
										procfuncptr->substopic[i].handle=tempaddr;
										procfuncptr->substopic[i].groupID=tempgroup;
										
									}	
    		
						}	

  		k++;

	}
	restore(mask);
	return OK;
}
