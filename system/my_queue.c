#include <xinu.h>
#include <my_queue.h>

syscall init_q(queue *q)
{
  q->head = 0;
  q->tail = 0;
  return OK;
}

int is_empty(queue *q)
{
  if(q->head == q->tail)
    return 0;
  
  return 1;
}

pid32 first_id(queue *q)
{
    if(q->head == q->tail)
    {
      q->head = 0;
      q->tail = 0;
      return SYSERR;
    }
      
    pid32 pid = q->qu[q->head];
    return pid;
}

syscall enq(queue *q, pid32 pid)
{
  if(q->tail == Q_MAX_COUNT)
  {
    return SYSERR;
  }
  
  q->qu[q->tail] = pid;
  q->tail++;
  return OK;
}

pid32 deq(queue *q)
{
  if(q->head == q->tail)
  {
    q->head = 0;
    q->tail = 0;
    return SYSERR;
  }
  
  pid32 pid = q->qu[q->head];
  q->head++;
  
  return pid;
}


  
  
  