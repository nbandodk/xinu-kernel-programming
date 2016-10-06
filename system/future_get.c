#include <future.h>
#include <kernel.h>
#include <prodcons.h>

syscall future_get(future *f, int *value)
{
	if(f->state ==  FUTURE_EMPTY)
	{
		f->state = FUTURE_WAITING;
		f->pid = getpid();
		return OK;
	}
	
	if(f->state == FUTURE_WAITING)
	{
		return SYSERR;
	}
	
	if(f->state == FUTURE_VALID)
	{
		*value = f->value;
		f->pid = NULL;
		f->state = FUTURE_EMPTY;
		return OK;
	}

	return OK;
}
