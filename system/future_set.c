#include<xinu.h>
#include<future.h>

syscall future_set(future *f, int *value)		//Set the value to future
{
	int pid;
	// intmask interrupt;

// ********************************FUTURE_EXCLUSIVE********************************

	if(f->flag == FUTURE_EXCLUSIVE)			//Check if flag is exclusive
	{
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)	
		{
			*(f->value) = *(value);
			// f->value = value;
			f->state = FUTURE_VALID;
			// interrupt = disable();
			ready(f->pid);		//Put the process back in the ready queue

			// restore(interrupt);
			return OK;
		}
	
		if(f->state == FUTURE_VALID)
		{
			return SYSERR;
		}	
	}

	// ************************FUTURE_SHARED***************************************

	if(f->flag == FUTURE_SHARED)
	{
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
		{
			f->value = value;
			f->state = FUTURE_VALID;
			struct qent *current;
			current = &f->get_queue;

			while(current != NULL)
			{	
			 	ready(current->qpid);
				current = current->qnext;
				freemem(current, sizeof(struct qent));		//Free memory


				f->state = FUTURE_VALID;
			}
		}

		if(f->state == FUTURE_VALID)
		{
			return SYSERR;
		}
	}

// **************************FUTURE_QUEUE***************************************

	if(f->flag == FUTURE_QUEUE)
	{
		if(f->get_queue.qnext != NULL)
		{
			struct qent *current;

			current = &f->get_queue;
			current = current->qnext;
			current->value = *value;

			f->get_queue.qnext = current->qnext;		
			resume(current->qpid);
			freemem(current, sizeof(struct qent));		//Free memory
			
			return OK;
		}

		if(f->get_queue.qnext == NULL)
		{
			struct qent *current, *new_node;
			pid = getpid();

			new_node = (struct qent*)getmem(sizeof(struct qent));
			new_node->qpid = pid;
			new_node->qnext = NULL;

			current = &f->set_queue;

			while(current->qnext != NULL)
			{
				current = current->qnext;
			}

			current->qnext = new_node;
			*value = new_node->value;

			// proctab[f->pid].prstate = PR_WAIT;		//Put the process into wait state
			// resched();
			suspend(new_node->qpid);
			return OK;	
		}
	}
}