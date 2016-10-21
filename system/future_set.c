#include <xinu.h>
#include <future.h>
#include <kernel.h>
#include <prodcons.h>

syscall future_set(future *f, int *value){

  switch(f->flag){

      case 1:
      
        if(f->state == FUTURE_WAITING){

        f->value = *value;
        f->state = FUTURE_VALID;
        resume(f->pid);
        return OK;
      }

      if(f->state == FUTURE_EMPTY){

        f->value = *value;
        f->state = FUTURE_VALID;

        return OK;
      }

      if(f->state == FUTURE_VALID){
        return SYSERR;
      }
      break;
      
      case 2:

        if(f->state == FUTURE_WAITING){

          f->value = *value;
          f->state = FUTURE_VALID;
          f->pid = deq(&f->get_queue);
          resume(f->pid);
        }

        if(f->state == FUTURE_EMPTY){

          f->value = *value;
          f->state = FUTURE_VALID;

          return OK;
        }

        if(f->state == FUTURE_VALID){
          return SYSERR;
        }
        break;

  }
  return OK;
}
