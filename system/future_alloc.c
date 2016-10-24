
#include <future.h>

future* future_alloc(int fut_flag){

  intmask mask;
  mask = disable();
  
  //kprintf("Inside allocate \n");
  //allocate memory to future
  future *f=getmem(sizeof(future));
  
  //validate future reference
  if ((int*)f == -1){
    return NULL;
  }

  f->set_queue = getmem(sizeof(struct Queue));
  f->get_queue = getmem(sizeof(struct Queue));
  //set future values
  f->flag = fut_flag;
  f->state = FUTURE_EMPTY;
  f->pid = NULL;
  f->value = 0;
  init_q(&f->set_queue);
  init_q(&f->get_queue);

  restore(mask);
  
  //kprintf("Exiting allocate \n");
  return f;
  
}
