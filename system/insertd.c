/* insertd.c - insertd */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insertd  -  Insert a process in delta list using delay as the key
 *------------------------------------------------------------------------
 */
status	insertd(			/* Assumes interrupts disabled	*/
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Delay from "now" (in ms.)	*/
	)
{
	
	struct qentry *new_node, *prev, *next;

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	prev = &queuetab[queuehead(q)]; 	// Assigns head to prev
	next = queuetab[queuehead(q)].qnext;	// Assigns next to the first node after head node
	while ((next->pid != queuetail(q)) && (next->qkey <= key)) { 	// Look for the node in the list
		
		key -= next->qkey;
		prev = next;
		next = next->qnext;

	}

	/* Insert new node between prev and next nodes */

	
	new_node = (struct qentry*)getmem(sizeof(struct qentry)); 	// Allocate memory for new node
	new_node->qkey = key;
	new_node->pid = pid;

	new_node->qnext = next;
	new_node->qprev = prev;
	prev->qnext = new_node;
	next->qprev = new_node;

	if (next->pid != queuetail(q)) {
	
		next->qkey -= key;
	}

	return OK;
}
