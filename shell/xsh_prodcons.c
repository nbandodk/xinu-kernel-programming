#include <xinu.h>
#include <prodcons.h>
#include <future.h>

int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced;
sid32 consumed;

shellcmd xsh_prodcons(int nargs, char *args[]){
  int count = 2000;             //local varible to hold count

  //Argument verifications and validations
  if(nargs < 2)
  {
      produced = semcreate(0);
      consumed = semcreate(1);

      //create the process producer and consumer and put them in ready queue.
      resume( create(produce, 1024, 20, "producer", 1, count));
      resume( create(consume, 1024, 20, "consumer", 1, count));
  }
  if (nargs > 2) {
    fprintf(stderr, "%s: too many arguments\n", args[0]);
    fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
    return 1;
  }

  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
    printf("Usage: %s <count>  OR %s -f\n\n", args[0],args[0]);
    return 0;
  }

  //check args[1] if present and take appropriate action
  if (nargs == 2){
    if(strncmp(args[1], "-f",2)==0){
      
      
      /*future *f1, *f2, *f3;
      f1 = future_alloc(FUTURE_EXCLUSIVE);
      f2 = future_alloc(FUTURE_EXCLUSIVE);
      f3 = future_alloc(FUTURE_EXCLUSIVE);
      resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
      resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
      resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
      resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
      resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
      resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );*/
      
      future *f_shared;
      f_shared = future_alloc(FUTURE_SHARED);
      
      // Test FUTURE_SHARED
      resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
      resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
      resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
      resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
      resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );
    }
    
    else{
    
      count = atoi(args[1]);
      produced = semcreate(0);
      consumed = semcreate(1);

      //create the process producer and consumer and put them in ready queue.
      resume( create(produce, 1024, 20, "producer", 1, count));
      resume( create(consume, 1024, 20, "consumer", 1, count));
    }
  }

  return (0);
}
