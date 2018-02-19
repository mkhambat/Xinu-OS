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

	
	current = &queuetab[queuehead(q)];		//Assign the head node of the list to current
	while (current!=NULL && current->qkey>=key)		//Look for a node which has priority less than key
	{
		current = current->qnext;
	}

	// new_node = &queuetab[pid];
	new_node = (struct qentry*)getmem(sizeof(struct qentry)); 	//Allocate memory for a new node
	new_node->qkey = key; 	
	new_node->pid = pid;
	// Insert node between current node and prev node
	new_node->qnext = current;
	new_node->qprev = current->qprev;
	(current->qprev)->qnext = new_node;
	current->qprev = new_node;

	return OK;
}