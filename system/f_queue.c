#include<xinu.h>
#include<future.h>


// struct qent f_newqueue(void)
// {
// 	struct qent *head;
// 	struct qent *tail;

// 	head = (struct qent*)getmem(sizeof(struct qent));
// 	tail = (struct qent*)getmem(sizeof(struct qent));
// 	head->qpid = 999;
// 	head->qnext = tail;
// 	tail->qnext = NULL;
// 	tail->qpid = -999;
// 	return head;
// }

void f_enqueue(struct qent *head,pid32 pid)
{
	struct qent *current;
	struct qent *new_node;

	new_node  = (struct qent*)getmem(sizeof(struct qent));
	new_node->qpid = pid;

	current = head;
	while(current->qnext->qpid!=NULL)
	{
		current=current->qnext;
	}
	new_node->qnext = current->qnext;
	current->qnext = new_node;
}

pid32 f_dequeue(struct qent *head)
{
	if(head->qnext->qpid == NULL)
	{
		return SYSERR;
	}
	else
	{
		struct qent *current;
		current = head->qnext;
		head->qnext = current->qnext;
		freemem(current, sizeof(struct qent));
		return (current->qpid);
	}
}