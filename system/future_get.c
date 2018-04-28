#include<xinu.h>
#include<future.h>

syscall future_get(future *f, int *value)		//Get a value of the future and store in "value"
{
	// struct futent *new_future;
	int pid;
	// intmask interrupt;

	// *********************************FUTURE_EXCLUSIVE***********************************

	if(f->flag == FUTURE_EXCLUSIVE)		//Check if flag of future is exclusive 
	{
		if(f->state == FUTURE_EMPTY)	//Check if state is empty
		{
			pid = getpid();			//Get the pid of the calling process
			// interrupt = disable();	//Disable all interrupts
			// suspend(pid);
			f->pid = pid;
			f->state = FUTURE_WAITING;
			proctab[f->pid].prstate = PR_WAIT;		//Put the process into wait state
			resched();			//Call reched
			// restore(interrupt);		//Restore all interrupts

			if(f->state == FUTURE_VALID)	//Check if state is valid
			{	
				*value = *f->value;		//Get the value of future
				return OK;
			}
		}
		
		
		if(f->state == FUTURE_VALID)		//Check if state is valid
		{
			// interrupt = disable();			//Disable all interrupts
			*value = *f->value;			//Get the value of future
			f->state = FUTURE_EMPTY;		//Set state to empty
			// restore(interrupt);				//Restore all interrupts
			return OK;
		}
	}

	// **************************FUTURE_SHARED*********************************************

	if(f->flag == FUTURE_SHARED)
	{
		if(f->state == FUTURE_EMPTY)
		{
			f->state = FUTURE_WAITING;
			pid = getpid();
			struct qent *new_node;
			// Allocate memory for new node
			new_node = (struct qent*)getmem(sizeof(struct qent));
			new_node->qpid = pid;
			new_node->qnext = NULL;
			f->get_queue = *new_node;

			proctab[pid].prstate = PR_WAIT;		//Put the process into wait state
			resched();
		}

		if(f->state == FUTURE_WAITING)
		{
			pid = getpid();
			struct qent *new_node, *current;

			new_node = (struct qent*)getmem(sizeof(struct qent));
			new_node->qpid = pid;
			new_node->qnext = NULL;

			current = &f->get_queue;
			// ENQUEUE
			while(current->qnext!=NULL)
			{
				current = current->qnext;
			}

			current->qnext = new_node;
			proctab[pid].prstate = PR_WAIT;		//Put the process into wait state
			resched();
		}

		if(f->state == FUTURE_VALID)
		{
			*value = (*f->value);
			return OK;
		}
	}

// ***************************FUTURE_QUEUE**********************************************

	if(f->flag == FUTURE_QUEUE)
	{
		if(f->set_queue.qnext != NULL)
		{

			struct qent *current;
			current = &f->set_queue;
			current = current->qnext;
			*value = current->value;
			// DEQUEUE
			f->set_queue.qnext = current->qnext;
			resume(current->qpid);
			freemem(current, sizeof(struct qent));		//Free memory

			
			return OK;
		}

		if(f->set_queue.qnext == NULL)
		{
			struct qent *current, *new_node;
			pid = getpid();
			new_node = (struct qent*)getmem(sizeof(struct qent));
			new_node->qpid = pid;
			new_node->qnext = NULL;

			current = &f->get_queue;
			// ENQUEUE
			while(current->qnext != NULL)
			{
				current = current->qnext;	
			}

			current->qnext = new_node;
			// proctab[pid].prstate = PR_WAIT;		//Put the process into wait state
			// resched();

			suspend(pid);
			*value = new_node->value;
			return OK;
		}
	}

	else
	{
		return SYSERR;
	}
	
}