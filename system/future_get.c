
#include <future.h>

syscall future_get(future *f, int *value){

   intmask mask;
   mask = disable();

   switch(f->flag){

   case FUTURE_EXCLUSIVE:

      if(f->state ==  FUTURE_EMPTY){

        f->state = FUTURE_WAITING;
        f->pid = getpid();

        suspend(f->pid);
        resched();
        
        *value = f->value;
        
        restore(mask);
        return OK;
      }

      if(f->state == FUTURE_WAITING){
        restore(mask);
        return SYSERR;
      }

      if(f->state == FUTURE_VALID){

        *value = f->value;
        f->pid = getpid();
        
        restore(mask);
        return OK;
      }

      break;

   case FUTURE_SHARED:

      if(f->state ==  FUTURE_EMPTY){

        f->state = FUTURE_WAITING;
        f->pid = getpid();
        enq(&f->get_queue, f->pid);
        suspend(f->pid);
        resched();

        *value = f->value;
        
        restore(mask);
        return OK;
      }

      if(f->state == FUTURE_WAITING){
        f->pid = getpid();
        enq(&f->get_queue, f->pid);
        suspend(f->pid);
        resched();

        *value = f->value;
        
        restore(mask);
        return OK;
      }

      if(f->state == FUTURE_VALID){

        *value = f->value;
        
        restore(mask);
        return OK;
      }
      break;

  }

 restore(mask);
 return OK;
}
