#include <future.h>


uint future_cons(future *fut){

  int i,status;
  status = future_get(fut, &i);

  if (status < 1){
    printf(" Future_get failed\n");
    return -1;
  }

  //printf("Process id: %d consumed: %d\n",currpid, i);
  printf("it produced %d\n", i);
  
  //future_free(&fut);

  return OK;
}
