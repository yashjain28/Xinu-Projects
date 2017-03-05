/*  main.c  - main */

#include <xinu.h>
pid32 send_msg;
pid32 send_msgs;
pid32 send_N_msg;
pid32 receive_Msgs;
pid32 receive_Msg;

process SENDMSG(void)
{
	int i=0;
	for(i=0;i<3;i++){
		sendMsg(receive_Msg,i+10);
	}
	return OK;
}


process RECEIVEMSG(void)
{

	int i=0;
	umsg32 mesg;
	for(i=0;i<4;i++)
	{
		mesg=receiveMsg();
		//printf("mr=%d\n",mesg);
	}
	return OK;
}

process SENDMSGS(void)
{
	int i=0;
	uint32 success;
	umsg32 arr[10];
	for(i=0;i<10;i++){
			arr[i]=i+10;
	}
	success=sendMsgs(receive_Msgs,arr,12);
	//printf("case 1: success = %d\n",success);
	success=sendMsgs(receive_Msgs,arr,6);
	//printf("case 2: success = %d\n",success);
	 // success is SYSERR i.e. (-1) if no messages sent successfully
	return OK;
}

process SENDNMSGS(void)
{

	umsg32 mesg=34;
	pid32 pids[4];
	uint32 success;
	pids[0]=send_msg; 
	pids[1]= receive_Msg;
	pids[2]= send_msgs;
	//pids[5]= send_N_msg;
	pids[3]= receive_Msgs;
 	success=sendnMsg(4,pids,mesg);
 	//printf("successNmsgs = %d\n",success);
	return OK;
}
process RECEIVEMSGS(void)
{

	umsg32 a[10];
	receiveMsgs(a,5);
	return OK;
}



process	main(void)
{
	recvclr();

	send_msg=create(SENDMSG, 4096, 50, "sendMsg",0);
	receive_Msg=create(RECEIVEMSG, 4096, 50, "receiveMsg",0);
	send_msgs=create(SENDMSGS, 4096, 50, "sendMsgs",0);
	receive_Msgs=create(RECEIVEMSGS, 4096, 50, "receiveMsgs",0);
	send_N_msg=create(SENDNMSGS, 4096, 50, "sendnMsg",0);
	
	resched_cntl(DEFER_START);
	resume(send_msg);
	resume(receive_Msg);
	resume(send_msgs);
	resume(receive_Msgs);
	resume(send_N_msg);
	resched_cntl(DEFER_STOP);
	return OK;
}