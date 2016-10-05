#include <future.h>

syscall future_set(future *f, int *value)
{
	if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
	{
		f->value = *value;
		f->state = FUTURE_VALID;
		return OK;
	}

	if(f->state == FUTURE_VALID)
	{
		return SYSERR;
	}

	return OK;
}
