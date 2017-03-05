#include <xinu.h>

syscall  handlerprint(topic16  topic,uint32 mesg){

	intmask 	mask;    	/* stores new process id	*/
	struct	topicent	*procfuncptr;		/* pointer to proc. table entry */
	int i,count;
	mask=disable();
	if (isbadtopic(topic & 255)) {
		restore(mask);
		return SYSERR;
	}
	procfuncptr=&topictab[(topic & 255)];
	i=0;
	count=0;
	/*kprintf("\t");
	for(i=0;i<8;i++){
		kprintf("%d\t",procfuncptr->substopic[i].procID);
	}
	kprintf("\n");*/
	while(procfuncptr->substopic[i].procID!=-1 && count<8){
		if((topic >> 8)==0){
			procfuncptr->substopic[i].handle(topic,mesg);
			kprintf("---proc %d received %d from group %d topic %d\n",procfuncptr->substopic[i].procID,mesg,(topic>>8),(topic&255));
		}
		else if((topic >> 8)==procfuncptr->substopic[i].groupID){
			procfuncptr->substopic[i].handle(topic,mesg);
			kprintf("---proc %d received %d from group %d topic %d\n",procfuncptr->substopic[i].procID,mesg,(topic>>8),(topic&255));
		}		
		i++;
		count++;
	}
	if(i==0 || count==8){
		restore(mask);
		return SYSERR;
	}
	restore(mask);
	return OK;

}