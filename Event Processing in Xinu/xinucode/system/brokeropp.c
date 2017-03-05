/* brokeropp.c - addnodeblist, delnodeblist */

#include <xinu.h>
	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  addbrokerlist  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
syscall	addnodeblist( topic16 topicid, uint32 mesg){

	struct brokernode *newnode=NULL;
	struct brokernode temp;
	struct brokernode *iter=NULL;
	intmask mask;
	mask=disable();
	if (isbadtopic(topicid & 255)) {
		restore(mask);
		return SYSERR;
	}
	newnode=(struct brokernode*)getmem(sizeof(temp));
	if(headbroker==NULL){
		headbroker=newnode;
		//kprintf("DaddedBLatHEAD %d\t\t",topicid);
	}
	else{
			iter=headbroker;
			while(iter->next!=NULL){
				iter=iter->next;
			}
			iter->next=newnode;
	}		
		newnode->prev=iter;
		newnode->next=NULL;
		newnode->procid=currpid;
		newnode->data=mesg;
		newnode->topicID=topicid;
		//kprintf("DaddedBL %d\t\t",topicid);
	restore(mask);
	return OK;
}

syscall delnodeblist()
{
	struct brokernode temp;
	struct brokernode *iter=NULL;
	if (headbroker==NULL) {
		return SYSERR;
	}
	else{
			iter=headbroker;
			headbroker=iter->next;
			freemem((char *)iter,sizeof(temp));
			//kprintf("\tmem freed\t");
	}
	return OK;
}

syscall printblist(){
	struct brokernode *iter=NULL;
	intmask mask;
	mask=disable();
	if (headbroker==NULL) {
		kprintf("EmptyList\t");			
		restore(mask);
		return SYSERR;
	}
	else{
		iter=headbroker;
			while(iter!=NULL){
				iter=iter->next;
				kprintf("%d\t",iter->data);
			}
		kprintf("\n");
			
	}
	restore(mask);
	return OK;
}