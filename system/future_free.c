
#include <future.h>


syscall future_free(future* f){
  
  intmask mask;
  mask = disable();
  //release alloted memory for future.
  int ret = freemem(f, sizeof(future));

  if(ret == SYSERR){
    restore(mask);
    return  ret;
  }
  else{
    restore(mask);
    return OK;
  }
}
