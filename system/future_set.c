
#include <future.h>


syscall future_set(future *f, int *value){

  intmask mask;
  mask = disable();

  switch(f->flag){

      case FUTURE_EXCLUSIVE:
      
        if(f->state == FUTURE_WAITING){

        f->value = *value;
        f->state = FUTURE_VALID;
        resume(f->pid);
        resched();
        
        restore(mask);
        return OK;
      }

      if(f->state == FUTURE_EMPTY){

        f->value = *value;
        f->state = FUTURE_VALID;
        
        restore(mask);
        return OK;
      }

      if(f->state == FUTURE_VALID){
      
        restore(mask);
        return SYSERR;
      }
      
      
      break;
      
      case FUTURE_SHARED:

        if(f->state == FUTURE_WAITING){
          
          pid32 pro_id;
          f->value = *value;
          f->state = FUTURE_VALID;
          
          while(!is_empty(&f->get_queue))
          {
            pro_id = deq(&f->get_queue);
            resume(pro_id);
          }
          restore(mask);
          return OK;
        }

        if(f->state == FUTURE_EMPTY){

          f->value = *value;
          f->state = FUTURE_VALID;
          
          restore(mask);
          return OK;
        }

        if(f->state == FUTURE_VALID){
        
          restore(mask);
          return SYSERR;
        }
        break;

  }
  return OK;
}
