#include<xinu.h>
#include<future.h>

syscall future_get(future *f, int *value)		//Get a value of the future and store in "value"
{
	// struct futent *new_future;
	int pid;
	intmask interrupt;
	if(f->flag == FUTURE_EXCLUSIVE)		//Check if flag of future is exclusive 
	{
		if(f->state == FUTURE_EMPTY)	//Check if state is empty
		{
			pid = getpid();			//Get the pid of the calling process
			interrupt = disable();	//Disable all interrupts
			// suspend(pid);
			f->pid = pid;
			f->state = FUTURE_WAITING;
			proctab[f->pid].prstate = PR_WAIT;		//Put the process into wait state
			resched();			//Call reched
			restore(interrupt);		//Restore all interrupts

			if(f->state == FUTURE_VALID)	//Check if state is valid
			{	
				*value = *(f->value);		//Get the value of future
				f->state = FUTURE_EMPTY;	//Set its state to empty
				return OK;
			}
		}
		
		// if(f->state == FUTURE_WAITING)
		// {
		// 	return SYSERR;
		// }
		
		if(f->state == FUTURE_VALID)		//Check if state is valid
		{
			interrupt = disable();			//Disable all interrupts
			*value = *(f->value);			//Get the value of future
			f->state = FUTURE_EMPTY;		//Set state to empty
			restore(interrupt);				//Restore all interrupts
			return OK;
		}
	}
	else
	{
		return SYSERR;
	}
	
}