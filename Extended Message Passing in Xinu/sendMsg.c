/* sendMsg.c - sendMsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendMsg  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	sendMsg(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	if(prptr->empty>0)
		{
			prptr->prmsg1[prptr->nextempty] = msg;
			prptr->nextempty=(prptr->nextempty+1)%10;
			prptr->empty--;
			prptr->full++;
			printf("sendMsg %d to PID=%d\n",msg,pid);
		}
	else{
			restore(mask);
			return SYSERR;
		}		
	/* if a message is sent */	
	/* Indicate message is waiting	*/
	/* If recipient waiting or in timed-wait make it ready */
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
