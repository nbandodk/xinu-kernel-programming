#include <xinu.h>
#include <prodcons.h>

int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced;
sid32 consumed;

shellcmd xsh_prodcons(int nargs, char *args[])
{
	int count = 2000;             //local varible to hold count
	
	produced = semcreate(0);
	consumed = semcreate(1);
	
	//Argument verifications and validations
	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <count>\n\n", args[0]);
		return 0;
	}
	
	 //check args[1] if present assign value to count
	if (nargs == 2) {
		
		count = atoi(args[1]);
		
	
	}
  
  
  
  

 

  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.      
  resume( create(produce, 1024, 20, "producer", 1, count));
  resume( create(consume, 1024, 20, "consumer", 1, count));
  return (0);

}