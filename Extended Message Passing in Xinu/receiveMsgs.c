/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
syscall	receiveMsgs( umsg32* msgs, uint32	msg_count)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int i=0;		/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if(msg_count>10) 
	{	
		restore(mask);	
		return SYSERR;
	}
	while(prptr->full<msg_count)
	{
		prptr->prstate = PR_RECV;
		resched();		/* Block until message count no of messages arrive	*/
	}
	while(msg_count>0)
		{
			msgs[i++] = prptr->prmsg1[prptr->nextfull];
			prptr->prmsg1[prptr->nextfull]=-1;
			prptr->nextfull=(prptr->nextfull+1)%10;
			prptr->full--;
			prptr->empty++;
			msg_count--;
			printf("receiveMsgs= %d\t",(int)msgs[i-1]);
		}

	restore(mask);
	return OK;
}
