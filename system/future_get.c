
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

   case FUTURE_QUEUE:
   
       if(is_empty(&f->set_queue))
        {
          kprintf("\n If nothing in setq");
          f->state=1;
          enq(&f->get_queue, currpid);
          suspend(currpid);
          resched();
          *value = f->value;
          f->state=0;
        }
        else
        {
          if(f->state==2)
          {
            kprintf("\n If something in setq and state is valid");
            f->state=1;
            enq(&f->get_queue, currpid);
            suspend(currpid);
            resched();
            *value = f->value;
            f->state=0;
          }
          else
          {
            kprintf("\n If something in setq and state != Valid");
            pid32 temp_pid = deq(&f->set_queue);
            resume(temp_pid);
            //resched();
            sleep(2);
            printf("\n Get the value !!");
            *value = f->value;
            f->state=0;
          }
        }
        break;
        
          
          
          /*case FUTURE_QUEUE:
   
     if(is_empty(&f->set_queue))
        {
          enq(&f->get_queue, currpid);
          suspend(currpid);
          resched();
        }
        else
        {
          
          pid32 temp_pid = deq(&f->set_queue);
          resume(temp_pid);
          resched();
          *value = f->value;
        }
        break;*/
  }

 restore(mask);
 return OK;
}
