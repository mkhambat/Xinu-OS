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
	// pid32	head;
	// pid32 first_pid;
	struct qentry *first,*head;
	// first_pid = getitem(queuetab[head].qnext->pid)
	if (isempty(q) ) {
		return EMPTY;
	}

	if(isbadqid(q))
	{
		return SYSERR;
	}

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
	// pid32 tail;
	struct qentry *last, *tail; 
	
	if (isempty(q)) {
		return NULL;
	}

	if(isbadqid(q)){
		return SYSERR;
	}

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
	// pid32	prev, next;

	if (isbadpid(pid))
	{
		return NULL;
	}
	struct qentry *prev, *next, *current;	

	if(proctab[pid].prstate==PR_READY)
	{
		current = &queuetab[readylist];
		while(current!=NULL && current->pid!=pid)
		{
			current=current->qnext;
		}
	}

	if(proctab[pid].prstate==PR_SLEEP)
	{
		current = &queuetab[sleepq];
		while(current!=NULL && current->pid!=pid)
		{
			current=current->qnext;
		}
	}


	// current = &queuetab[pid];
	next = current->qnext;	
	prev = current->qprev;	
	// queuetab[prev->pid].qnext = next;
	// queuetab[next->pid].qprev = prev;
	prev->qnext = next;
	next->qprev = prev;

	freemem(current,sizeof(struct qentry));

	return pid;
}
