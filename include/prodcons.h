#include <xinu.h>
#include <future.h>

/*Global variable for producer consumer*/

extern int n; /*this is just declaration*/

/*function Prototype*/
void consume(int);
void produce(int);
uint future_prod(future *);
uint future_cons(future *);

/* Semaphore Declarations */
extern sid32 produced, consumed;
