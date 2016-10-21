#include <kernel.h>
#include <prodcons.h>
#include <future.h>

future* future_alloc(int fut_flag){

  //allocate memory to future
  future *f=getmem(sizeof(future));

  //validate future reference
  if ((int*)f == -1){
    return NULL;
  }

  //set future values
  f->flag = fut_flag;
  f->state = FUTURE_EMPTY;
  f->pid = NULL;
  Queue setq, getq;
  f->set_queue = init_q(setq);
  f->get_queue = init_q(getq);

  return f;
}
