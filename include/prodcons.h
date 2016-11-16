#include <xinu.h>


/*Global variable for producer consumer*/

extern int n; /*this is just declaration*/

/*function Prototype*/
void consume(int);
void produce(int);


/* Semaphore Declarations */
extern sid32 produced, consumed;
