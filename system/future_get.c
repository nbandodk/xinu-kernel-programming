#include <future.h>
#include <kernel.h>
#include <prodcons.h>

syscall future_get(future *f, int *value)
{
	if(f->state ==  FUTURE_EMPTY)
	{
		f->state = FUTURE_WAITING;
		f->pid = getpid();
		kprintf("suspending consumer %d\n",f->pid);			
		suspend(f->pid);		
		
		*value = f->value;
		printf("\n Giving the value %d \n", value);
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
		printf("\n Giving the value %d \n", value);
		return OK;
	}

	return OK;
}
