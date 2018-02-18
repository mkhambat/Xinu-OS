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

	// tail = queuetail(q);
	// prev = queuetab[tail->pid].qprev->pid;

	// queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	// queuetab[pid].qprev  = prev;
	// queuetab[prev->pid].qnext = pid;
	// queuetab[tail->pid].qprev = pid;

	// current = &queuetab[pid];
	// current = &queuetab[queuehead(q)];
	// while(pid!=NULL && q->pid!=pid)
	// {
	// 	current = current->qnext;
	// }

	current = (struct qentry*)getmem(sizeof(struct qentry));
	current->pid = pid;

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

	// head = &queuetab[queuehead(q)];
	// next = &queuetab[head->pid].qnext;

	// head.qnext = next;
	// head = next->prev;

	


	pid = getfirst(q);
	// current = &queuetab[pid];
	// // first=&queuetab[pid];
	// current->qprev = NULL;
	// current->qnext = NULL;

	// head->qnext = first->qnext;
	// next->qprev = first->qprev;

	// queuetab[head->pid].qnext->pid = queuetab[head].qnext
	return pid;
}
