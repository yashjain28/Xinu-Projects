/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendnMsg  -  broadcast an update message
 	Assuming that data in the reciever is overwrtitten if it has any data
 *------------------------------------------------------------------------
 */
uint32	sendnMsg(
	  uint32	pid_count,		/* ID of recipient process	*/
	  pid32*	pids,		
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int i=0,j=0;
	int badpid=0;
	int success=0;
	mask = disable();
	while(pid_count>0)
	{
		if (isbadpid(pids[i])) {
			badpid++;
			printf("Bad_pid=%d\t", pids[i]);
		}
		else
		{	
			prptr = &proctab[pids[i]];	
			if(prptr->empty>0)
			{
				prptr->prmsg1[prptr->nextempty] = msg;
				prptr->nextempty=(prptr->nextempty+1)%10;
				prptr->empty--;
				prptr->full++;
				printf("sendnMsgTOpid=%d msg=%d\t",pids[i],msg);
				success++;
				if (prptr->prstate == PR_RECV) {
						ready(pids[i]);
					} else if (prptr->prstate == PR_RECTIM) {
						unsleep(pids[i]);
						ready(pids[i]);
					}	
				}
		}
		i++;
		pid_count--;

	}
	/* If recipient waiting or in timed-wait make it ready */
	if(success==0){
	
		restore(mask);
		return SYSERR;
	}
	else{
		restore(mask);		/* Restore interrupts */
		return success;
	}
}
