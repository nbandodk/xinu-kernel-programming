#include <kernel.h>
#include <xinu.h>
#include <future.h>
#include <prodcons.h>

syscall future_free(future* f){

  //release alloted memory for future.
  int ret = freemem(f, sizeof(future));

  if(ret == SYSERR){
    return  ret;
  }
  else{
    return OK;
  }
}
