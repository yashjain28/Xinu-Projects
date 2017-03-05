/* recvMsg.c - recvMsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receiveMsg(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg=-5;			/* Message to return initialized with some value*/
	mask = disable();
	prptr = &proctab[currpid];
	while(prptr->full<=0)
	{
			prptr->prstate = PR_RECV;
			resched();		/* Block until message arrives	*/	
	}
	if(prptr->full>0)
	{
			msg=prptr->prmsg1[prptr->nextfull];
			prptr->prmsg1[prptr->nextfull]=-1;
			prptr->nextfull=(prptr->nextfull+1)%10;
			prptr->full--;
			prptr->empty++;
			printf("receiveMsg= %d\n",(int)msg);
	}
	restore(mask);
	return msg;
}
