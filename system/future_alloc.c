#include<xinu.h>
#include<future.h>

future* future_alloc(int future_flags)
{
	struct futent *new_future;
	new_future = (struct futent*)getmem(sizeof(struct futent));
	new_future->state = FUTURE_EMPTY;
	new_future->flag = future_flags;
	new_future->pid = NULL;
	new_future->value = NULL;

	return new_future;
}
