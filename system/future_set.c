#include<xinu.h>
#include<future.h>

syscall future_set(future *f, int *value)		//Set the value to future
{
	// int pid;
	// intmask interrupt;
	if(f->flag == FUTURE_EXCLUSIVE)			//Check if flag is exclusive
	{
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)	
		{
			*(f->value) = *value;
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
	
}