/* newqueue.c - newqueue */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  newqueue  -  Allocate and initialize a queue in the global queue table
 *------------------------------------------------------------------------
 */
qid16	newqueue(void)
{
	static qid16	nextqid=NPROC;	/* Next list in queuetab to use	*/
	qid16		q;		/* ID of allocated queue 	*/

	q = nextqid;
	if (q > NQENT) {		/* Check for table overflow	*/
		return SYSERR;
	}

	nextqid += 2;			/* Increment index for next call*/

	/* Initialize head and tail nodes to form an empty queue */

	queuetab[queuehead(q)].qnext = &queuetab[queuetail(q)];
	queuetab[queuehead(q)].qprev = NULL;
	queuetab[queuehead(q)].qkey  = MAXKEY;
	queuetab[queuehead(q)].pid = q;
	queuetab[queuetail(q)].qnext = NULL;
	queuetab[queuetail(q)].qprev = &queuetab[queuehead(q)];
	queuetab[queuetail(q)].qkey  = MINKEY;
	queuetab[queuetail(q)].pid = q+1;
	return q;
}


// qid newqueue_ll(void)
// {
// 	static qid16	nextqid=NPROC;
// 	qid16 q;
// 	q=nextqid;
// 	// nextqid = 3;

// 	q = nextqid;

// 	if(q>NQENT)
// 	{
// 		return SYSERR;
// 	}
// 	nextqid +=2;

// 	queuetab_ll[queuehead_ready(q)].qnext = &queuetab_ll[queuetail_ready(q)];
// 	queuetab_ll[queuehead_ready(q)].qprev = NULL;
// 	queuetab_ll[queuehead_ready(q)].qkey = MAXKEY;
// 	queuetab_ll[queuehead_ready(q)].pid = q;
// 	queuetab_ll[queuetail_ready(q)].qnext = NULL;
// 	queuetab_ll[queuetail_ready(q)].qprev = &queuetab_ll[queuehead_ready(q)];
// 	queuetab_ll[queuetail_ready(q)].qkey = MINKEY;
// 	queuetab_ll[queuetail_ready(q)].pid = q+1;

// 	return q;
// }