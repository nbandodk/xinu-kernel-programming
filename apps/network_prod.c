#include <future.h>

uint network_prod(future *fut, int32 slot) {
  int j;
  char buf[1024];
  
  int32 nbytes = udp_recv(slot, buf, 1024, 15000);
  if(nbytes < 0) {
    printf("Couldn't receive value\n");
    return -1;
  }
  else {
    printf("Value received: %s\n", buf);
    j = atoi(buf);
    future_set(fut, &j);
    return OK;
  }
}
    
  /*  
  do {
    
    int32 nbytes = udp_recv(slot, buf, 1024, 15000);
    if(nbytes < 0) {
      printf("Couldn't receive value\n");
      break;
    }
    printf("Value received: %s\n", buf);
    j = atoi(buf);
    future_set(fut, &j);
    return OK;
  }
  */