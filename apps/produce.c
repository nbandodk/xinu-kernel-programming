#include <xinu.h>
#include <prodcons.h>

void produce(int count) {

	int32 i;
	
	for(i =1; i <= count ; i++)
	{
		wait(consumed);
		n = i;
		signal(produced);
	}
}



