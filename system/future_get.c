#include <xinu.h>
#include <future.h>
#include <kernel.h>
#include <prodcons.h>

syscall future_get(future *f, int *value){

 switch(f->flag){

   case 1:

      if(f->state ==  FUTURE_EMPTY){

        f->state = FUTURE_WAITING;
        f->pid = getpid();

        suspend(f->pid);

        *value = f->value;

        return OK;
      }

      if(f->state == FUTURE_WAITING){
        return SYSERR;
      }

      if(f->state == FUTURE_VALID){

        *value = f->value;
        f->pid = NULL;
        f->state = FUTURE_EMPTY;

        return OK;
      }

      break;

   case 2:

      if(f->state ==  FUTURE_EMPTY){

        f->state = FUTURE_WAITING;
        f->pid = getpid();
        enq(f->get_queue, f->pid);
        suspend(f->pid);
        

        *value = f->value;

        return OK;
      }

      if(f->state == FUTURE_WAITING){
        f->pid = getpid();
        enq(f->get_queue, f->pid);
        suspend(f->pid);
        

        *value = f->value;

        return OK;
      }

      if(f->state == FUTURE_VALID){

        *value = f->value;
        return OK;
      }
      break;

  }

return OK;
}
