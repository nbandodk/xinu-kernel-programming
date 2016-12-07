
#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 1024
shellcmd xsh_nettest(int nargs, char *args[]) {
	
	
	udp_init();
	kprintf("\nUDP table initialized !!");
	char remoteip[] = "10.0.2.15";
	
	int32 slot =0;
	uint32 target_ip;
	dot2ip(remoteip,&target_ip);
	
	slot=udp_register(target_ip,5000,1600);
	
	kprintf("\nSlot obtained : %d",slot);
	udp_send(slot,args[1],strlen(args[1]));
	kprintf("\nMessage sent \n");
	char tempbuff[BUFFSIZE];
	
	uint32 server;
	server = udp_register(0, 0, 1665);
	
	
	
	int32 msglen = udp_recv(server,tempbuff,BUFFSIZE,15000);
	
	//int32 msglen = udp_recv(slot,tempbuff,BUFFSIZE,15000);
	
	printf("mesgln:%d\n",msglen);
	if(msglen < 0)
	{
		printf("TimeOut!!\n");		
	}
	else
	{
		printf("Message Received :%s\n",tempbuff);	
	}
	
	
	/*
	char remoteip[] = "10.0.2.15";
	char msg1[BUFFSIZE];
	udp_init();
	uint32 server;
	server = udp_register(0, 0, 5555);
	uint32 rrecv;
	rrecv = udp_recv(server, msg1, BUFFSIZE, 15000);
	if(rrecv==SYSERR){
			printf("\n Error receiving data");
			udp_release(server);
			udp_release(client);
			return  SYSERR;
		}
		else if(rrecv==TIMEOUT){
			printf("\n Request timed out!");
			udp_release(server);
			udp_release(client);
			return SYSERR;
		}
		//Successful reception of data
		else{
			printf("\nReceived message : "); 
			printf("%s\n", msg1);
	}
	
	*/
	
	
	
	return 0;
	
	
	
	
}
