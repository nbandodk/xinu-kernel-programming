#include <stdio.h>
#include <future.h>

syscall future_free(future* f)
{
	free(f->value);
	int ret = freemem(f, sizeof(future));
	if(ret == SYSERR)
		return  ret;
	else
		return OK;
}
