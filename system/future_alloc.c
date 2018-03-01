#include<xinu.h>
#include<future.h>

future* future_alloc(int future_flags) 	//Allocates memory for a new future
{
	struct futent *new_future;
	new_future = (struct futent*)getmem(sizeof(struct futent)); 	//Allocates memory for a new future
	new_future->state = FUTURE_EMPTY; 	//Set future's state to empty
	new_future->flag = future_flags;	//Set future's flag to the input flag recieved
	new_future->pid = NULL;				//Set pid as null
	new_future->value = NULL;			//Set value as null

	return new_future;
}
