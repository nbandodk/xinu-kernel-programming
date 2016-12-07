#include <xinu.h>
#include <future.h>

#define BUFFSIZE 1024
shellcmd xsh_networkfutures(int nargs, char *args[]) {


	future *f_exclusive;	
	f_exclusive = future_alloc(FUTURE_EXCLUSIVE);

	udp_init();
	
	int32 slot = 0;
	
	slot = udp_register(0, 0, 1665);
	
	resume( create(network_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  	resume( create(network_prod, 1024, 20, "fprod1", 2, f_exclusive,slot) );
		
	return 0;
	
}