
#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 1024

shellcmd xsh_nettest(int nargs, char *args[]) {
	
	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}
	
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <type your message here>\n\n", args[0]);
		return 0;
	}
	
	if (nargs < 2) {
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

	if (nargs == 2) {
		
		
		udp_init();
	
	char remoteip[] = "10.0.2.15";
	
	int32 slot =0;
	uint32 target_ip;
	
	dot2ip(remoteip,&target_ip);
	
	slot = udp_register(target_ip,5000,1600);
	
	
	udp_send(slot,args[1],strlen(args[1]));
	kprintf("\nMessage sent \n");
	char buffer[BUFFSIZE];
	
	uint32 server;
	server = udp_register(0, 0, 1665);
	
	
	
	int32 nbytes = udp_recv(server,buffer,BUFFSIZE,15000);
	
	
	
	
	if(nbytes < 0)
	{
		printf("TimeOut!!\n");		
	}
	else
	{
		printf("Message Received :%s\n",buffer);	
	}
	return 0;
	
	}
	
	
	
	
	
	
	
	return 0;
	
	
	
	
}
