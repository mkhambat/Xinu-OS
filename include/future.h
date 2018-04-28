// #ifndef _FUTURE_H_
// #define _FUTURE_H_
 
// /* define states */
// #define FUTURE_EMPTY	  0
// #define FUTURE_WAITING 	  1         
// #define FUTURE_VALID 	  2         

// /* modes of operation for future*/
// #define FUTURE_EXCLUSIVE  0	

// typedef struct futent
// {
//    int *value;		
//    int flag;		
//    int state;         	
//    pid32 pid;
// } future;

// /* Interface for system call */
// future* future_alloc(int future_flags);
// syscall future_free(future*);
// syscall future_get(future*, int*);
// syscall future_set(future*, int*);
 
// #endif /* _FUTURE_H_ */

#include<xinu.h>

#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED     2	    
#define FUTURE_QUEUE      3     

struct qent	/* One per process plus two per list	*/
{	
	pid32	qpid;		 //Key on which the queue is ordered	
	struct qent *qnext;		/* Index of next process or tail	*/
	int value;
};

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
   struct qent set_queue;     
   struct qent get_queue;
}future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);


 
#endif /* _FUTURE_H_ */