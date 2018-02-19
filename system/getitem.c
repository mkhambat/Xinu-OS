/* getitem.c - getfirst, getlast, getitem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getfirst  -  Remove a process from the front of a queue
 *------------------------------------------------------------------------
 */
pid32	getfirst(
	  qid16		q		/* ID of queue from which to	*/
	)				/* Remove a process (assumed	*/
					/*   valid with no check)	*/
{
	
	struct qentry *first,*head;
	
	if (isempty(q) ) {
		return EMPTY;
	}

	if(isbadqid(q))
	{
		return SYSERR;
	}

	// Get the pid of the first node after head node
	head = &queuetab[queuehead(q)];
	first = head->qnext;
	return getitem(first->pid);
}

/*------------------------------------------------------------------------
 *  getlast  -  Remove a process from end of queue
 *------------------------------------------------------------------------
 */
pid32	getlast(
	  qid16		q		/* ID of queue from which to	*/
	)				/* Remove a process (assumed	*/
					/*   valid with no check)	*/
{
	
	struct qentry *last, *tail; 
	
	if (isempty(q)) {
		return NULL;
	}

	if(isbadqid(q)){
		return SYSERR;
	}

	// Get the pid of the first node before tail node
	tail = &queuetab[queuetail(q)];
	last = tail->qprev;
	return getitem(last->pid);
}

/*------------------------------------------------------------------------
 *  getitem  -  Remove a process from an arbitrary point in a queue
 *------------------------------------------------------------------------
 */
pid32	getitem(
	  pid32		pid		/* ID of process to remove	*/
	)
{
	

	if (isbadpid(pid))
	{
		return NULL;
	}
	struct qentry *prev, *next, *current;	

	if(proctab[pid].prstate==PR_READY)		// Check if process is in ready queue
	{
		current = &queuetab[readylist];
		while(current!=NULL && current->pid!=pid) 	// If found search for it
		{
			current=current->qnext;
		}
	}

	if(proctab[pid].prstate==PR_SLEEP) 		// Check if process is in sleep queue
	{
		current = &queuetab[sleepq];
		while(current!=NULL && current->pid!=pid)	// if found, search for it
		{
			current=current->qnext;
		}
	}


	// Remove a node from the queue
	next = current->qnext;	
	prev = current->qprev;	
	
	prev->qnext = next;
	next->qprev = prev;
	// Free its memory
	freemem(current,sizeof(struct qentry));

	return pid;
}
