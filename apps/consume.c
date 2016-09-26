#include <xinu.h>
#include <prodcons.h>

void consume(int count) {

	int32 i;
	
	for(i=1 ; i <= count ; i++)
	{
		wait(produced);
		printf("The value of n is %d \n", n);
		signal(consumed);
	}
}

