#include <future.h>

uint network_cons(future *fut) {
  int i, status;
  
  status = future_get(fut, &i);
  
  if (status < 1){
    printf(" Future_get failed\n");
    return -1;
  }
  
  printf("it produced %d\n", i);
  
}