#include<xinu.h>
#include<future.h>

syscall future_get(future *f, int *value)
{
	// struct futent *new_future;
	int pid;
	intmask interrupt;
	if(f->flag == FUTURE_EXCLUSIVE)
	{
		if(f->state == FUTURE_EMPTY)
		{
			pid = getpid();
			interrupt = disable();
			// suspend(pid);
			f->pid = pid;
			f->state = FUTURE_WAITING;
			proctab[f->pid].prstate = PR_WAIT;
			resched();
			restore(interrupt);

			if(f->state == FUTURE_VALID)
			{	
				*value = *(f->value);
				f->state = FUTURE_EMPTY;
				return OK;
			}
		}
		
		// if(f->state == FUTURE_WAITING)
		// {
		// 	return SYSERR;
		// }
		
		if(f->state == FUTURE_VALID)
		{
			interrupt = disable();
			*value = *(f->value);
			f->state = FUTURE_EMPTY;
			restore(interrupt);
			return OK;
		}
	}
	else
	{
		return SYSERR;
	}
	
}