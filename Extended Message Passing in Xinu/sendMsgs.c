/* sendMsg.c - sendMsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendMsg  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
uint32 sendMsgs(pid32 pid, umsg32* msgs, uint32 msg_count)
{
  intmask mask;     /* Saved interrupt mask   */
  struct  procent *prptr;   /* Ptr to process's table entry */
  int32   success=0; //count of no of sucessful meesages sent
  int i=0;
  mask = disable();

  if (isbadpid(pid)){
      restore(mask);
      return SYSERR;
  }

  prptr = &proctab[pid];

  while(prptr->empty<msg_count) //making the system atomic
  {
      restore(mask);
      return SYSERR;
  }
  printf("sendMsgs to PID=%d\t",pid);
  while(msg_count>0)
  {
    if(prptr->empty>0)
    {
            prptr->prmsg1[prptr->nextempty] = msgs[i++];
            prptr->nextempty=(prptr->nextempty+1)%10;
            prptr->empty--;
            prptr->full++;
            printf("sendMsgs %d\t",(int)msgs[i-1]);
            msg_count--;
            success++;
    }

  }
  printf("successfully sent: %d msgs\n",success);
  /* Indicate message is waiting  */
  /* If recipient waiting or in timed-wait make it ready */
  if(success>0)
  {
      if (prptr->prstate == PR_RECV) {
        ready(pid);
      } else if (prptr->prstate == PR_RECTIM) {
        unsleep(pid);
        ready(pid);
      }
  }    
  restore(mask);    /* Restore interrupts */
  return success;
}
