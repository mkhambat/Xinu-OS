#include<xinu.h>
#include<future.h>

syscall future_free(future *f)
{
	intmask interrupt;
	interrupt = disable();
	freemem(f, sizeof(struct futent));
	restore(interrupt);
	return OK;
}