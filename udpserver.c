/*
 * gcc -o udpserver udpserver.c
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sock;
        int addr_len, bytes_read;
        int serverPort;
        char recv_data[1024];
        struct sockaddr_in server_addr , client_addr;
        
        if(argc != 2){

            fprintf(stderr,"Invalid input\nUsage: ./a.out udpserver [Listen Port]");
            exit(1);
        }
        
        if(serverPort > 65535 )
        {
            perror("Server port exceeds 16-bit range\n");
            exit(1);
        }
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(serverPort);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);


        if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);
		
	printf("UDPServer Waiting for client on port X\n");
        fflush(stdout);

	while (1)
	{

          bytes_read = recvfrom(sock,recv_data,1024,0,
	                    (struct sockaddr *)&client_addr, &addr_len);
	  

	  recv_data[bytes_read] = '\0';

          printf("(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),
                                       ntohs(client_addr.sin_port));
          printf("%s", recv_data);
	  fflush(stdout);

        }
        return 0;
}
