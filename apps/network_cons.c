#include <future.h>

uint network_cons(future *fut, int32 slot) {
  int i, status;
  
  status = future_get(fut, &i);
  
  if (status < 1){
    printf(" Future_get failed\n");
    return -1;
  }
  
  printf("In network consumer ...... it produced %d\n", i);
  char ack_m[] = "Network consumer has consumed the value successfully.";
  udp_send(slot, ack_m, strlen(ack_m));
  
}