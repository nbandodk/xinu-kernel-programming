#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <memory.h>

future* future_alloc(int fut_flag)
{
	future *f=getmem(sizeof(future));
	if ((int*)f==-1)
		return NULL;
	
	f->flag=future_flag;
	f->state=FUTURE_EMPTY;
	f->pid=NULL;
	f->value=malloc(4);

	printf("Future is allocated . Flag=%d", f->flag);
	return f;
}

