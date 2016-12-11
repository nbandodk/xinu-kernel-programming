#include <xinu.h>
#include <future.h>

#define BUFFSIZE 1024
shellcmd xsh_networkfutures(int nargs, char *args[]) {

	if (nargs > 3) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <remote_port> <local_port>\n\n", args[0]);
		return 0;
	}
	
	if (nargs < 3) {
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

	if (nargs == 3) {
	
		future *f_exclusive;	
		f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
	
		udp_init();
		
		char remoteip[] = "10.0.2.15";
	
		int32 slot =0;
		uint32 target_ip;
		
		dot2ip(remoteip,&target_ip);
	
		slot = udp_register(target_ip, atoi(args[1]), 0);
	
	
		
	
		
		uint32 server;
		server = udp_register(0, 0, atoi(args[2]));
		
		
		
		resume( create(network_cons, 1024, 20, "fcons1", 2, f_exclusive, slot) );
	  	resume( create(network_prod, 1024, 20, "fprod1", 2, f_exclusive, server) );
			
		return 0;
	}
	
	return 0;
	
}