#include <future.h>
#include <xinu.h>
#include <kernel.h>
#include <prodcons.h>

uint future_prod(future *fut){
  int i, j;
  j = (int)fut;

  for (i=0; i<1000; i++) {
    j += i;
  }

  printf("\n Produced value %d \n", j);
  future_set(fut, &j);

  return OK;
}
