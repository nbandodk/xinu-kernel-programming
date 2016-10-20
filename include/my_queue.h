
#define Q_MAX_COUNT 20

typedef struct Queue
{
  int head;
  int tail;
  pid32 qu[Q_MAX_COUNT];
}queue;

syscall init_q(queue *q);
int is_empty(queue *q);
pid32 first_id(queue *q);
syscall enq(queue *q, pid32 pid);
pid32 deq(queue *q);

