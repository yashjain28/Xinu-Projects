/* brokerlist.h */
struct	brokernode  {		
	topic16	topicID;		
	uint32	data;
	pid32 procid;		
	struct brokernode *prev;
	struct brokernode *next;	
};

extern struct brokernode *headbroker;
extern struct brokernode *head1;
