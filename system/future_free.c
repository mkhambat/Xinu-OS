#include<xinu.h>
#include<future.h>

syscall future_free(future *f)		//Free memory of a future
{
	// intmask interrupt;
	// interrupt = disable();		//Disable all interrupts

	// freemem(f->)
	// freemem(f->get_queue, sizeof(f->get_queue));
	freemem(f, sizeof(struct futent));		//Free memory
	// freemem(f->pid, sizeof(f->pid));
	// freemem(f->state, sizeof(f->state));
	// freemem(f->value, sizeof(f->value));
	// freemem(f->flag, sizeof(f->flag));
	
	// restore(interrupt);			//Restore all interrupts
	return OK;
}