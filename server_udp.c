/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever 
   
   =======================================
   Code adapted from - linuxhowtos.org
   =======================================
   
*/

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   //----------------------SERVER SOCKET----------------------
   
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];

   if (argc < 3) {
      fprintf(stderr, "ERROR, USAGE: %s <local_port> <remote_port>\n", argv[0]);
      exit(0);
   }
   
   if (argc > 3) {
      fprintf(stderr, "ERROR, USAGE: %s <local_port> <remote_port>\n", argv[0]);
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   
   
   //------------------CLIENT SOCKET-------------------------------------------------
   
   int sockc, nc;
   unsigned int lengthc;
   struct sockaddr_in serverc, fromc;
   struct hostent *hp;
   char buffer[256];
   char hostt[]="192.168.1.101";
   
   sockc= socket(AF_INET, SOCK_DGRAM, 0);
   if (sockc < 0) error("socket");

   serverc.sin_family = AF_INET;
   hp = gethostbyname(hostt);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&serverc.sin_addr,
         hp->h_length);
   serverc.sin_port = htons(atoi(argv[2]));
   lengthc=sizeof(struct sockaddr_in);
   
   
   //-------------------------------------------------------------------
   
   while (1) {
       printf("Server initiated, waiting for incoming datagrams...........\n");
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       printf("\n");
       printf("Please enter your message: ");
       bzero(buffer,256);
       fgets(buffer,255,stdin);
       n=sendto(sockc,buffer,
                strlen(buffer),0,(const struct sockaddr *)&serverc,lengthc);
       if (n < 0) error("Sendto");
       printf("\n");
   }
   return 0;
 }

