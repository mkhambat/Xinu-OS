#include<xinu.h>
#include<future.h>

syscall future_free(future *f)		//Free memory of a future
{
	intmask interrupt;
	interrupt = disable();		//Disable all interrupts
	freemem(f, sizeof(struct futent));		//Free memory
	restore(interrupt);			//Restore all interrupts
	return OK;
}