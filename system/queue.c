/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	struct qentry	*tail, *prev, *current;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}


	current = (struct qentry*)getmem(sizeof(struct qentry));	//Allocate memory for a node
	current->pid = pid;		

	// Add a node just before the tail
	tail = &queuetab[queuetail(q)];
	prev = tail->qprev;

	current->qnext = tail;
	tail->qprev = current;
	current->qprev = prev;
	prev->qnext = current;


	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/
	struct qentry *current;
	// struct qentry *head, *next, *first;
	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return NULL;
	}

	
	pid = getfirst(q);		//Extract the pid of the 1st node after the head node
	
	return pid;
}
