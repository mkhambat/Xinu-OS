/* insert.c - insert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insert  -  Insert a process into a queue in descending key order
 *------------------------------------------------------------------------
 */
status	insert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
	struct qentry *current, *new_node;			/* Runs through items in a queue*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	// current = &queuetab[firstid(q)];
	// while (current != NULL && current->qkey >= key) {
	// 	current = queuetab[current->pid].qnext;
	// }

	// /* Insert process between curr node and previous node */

	// prev = queuetab[current->pid].qprev;	/* Get index of previous node	*/
	// queuetab[pid].qnext = current;
	// queuetab[pid].qprev = prev;
	// queuetab[pid].qkey = key;
 //    queuetab[pid].pid = pid;
	// queuetab[prev->pid].qnext = &queuetab[pid];
	// queuetab[current->pid].qprev = &queuetab[pid];

	current = &queuetab[queuehead(q)];
	while (current!=NULL && current->qkey>=key)
	{
		current = current->qnext;
	}

	// new_node = &queuetab[pid];
	new_node = (struct qentry*)getmem(sizeof(struct qentry));
	new_node->qkey = key;
	new_node->pid = pid;
	
	new_node->qnext = current;
	new_node->qprev = current->qprev;
	(current->qprev)->qnext = new_node;
	current->qprev = new_node;

	return OK;
}