#include<xinu.h>
#include<future.h>

syscall future_set(future *f, int *value)
{
	// int pid;
	intmask interrupt;
	if(f->flag == FUTURE_EXCLUSIVE)
	{
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			interrupt = disable();
			ready(f->pid);
			restore(interrupt);
			return OK;
		}
	
		if(f->state == FUTURE_VALID)
		{
			return SYSERR;
		}	
	}
	
}