/* topic.h - isbadtopic */

/* Maximum number of Topics in the system */

#ifndef NTOPIC
#define	NTOPIC		256
#endif		

#ifndef NSUBS
#define	NSUBS		8
#endif		

/* Inline code to checK topic number (assumes interrupts are disabled)	*/

#define	isbadtopic(x)	( ((topic16)(x) < 0) || \
			  ((topic16)(x) >= NTOPIC))

/* Definition of the topic table */

typedef struct procfunc {
		pid32 procID;
		int16 groupID;
		void (*handle)(topic16,  uint32);
}procfunc;

struct topicent {		

	procfunc substopic[NSUBS];
};
extern	struct	topicent topictab[];
