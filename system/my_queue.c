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
    return 1;
  
  return 0;
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
  //printf("Enqueue begin... head:%d  tail:%d\n",q->head,q->tail);
  if(q->tail == Q_MAX_COUNT)
  {
    return SYSERR;
  }
  
  q->qu[q->tail] = pid;
  q->tail++;
  
  //printf("Enqueue end... head:%d  tail:%d\n",q->head,q->tail);
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
  
  //printf("Dequeue... head:%d  tail:%d\n",q->head,q->tail);
  return pid;
}

syscall print_queue(queue *q)
{
  int i;
  if(q->head == q->tail)
    printf("Queue is empty \n");
  else
  {
    printf("Queue is : \n");
    for(i = q->head; i < q->tail; i++)
    {
      printf("%d ", q->qu[i]);
    }
    printf("\n");
  }
}
  
  
  