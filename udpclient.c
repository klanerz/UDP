/*
 * gcc -o udpclient udpclient.c
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char *argv[]) {

   int sock;
   char *hostInput;
   int clientPort;
   struct sockaddr_in server_addr;
   struct hostent *host;
   char send_data[1024];
   char quit[20];
   int ipVal = 0;
   unsigned long ip;

//check that correct number of arguments are given

   if(argc != 3){

    fprintf(stderr,"Incorrect number of arguments\nUsage: %s [host] [client]\n", argv[0]);

    return -1;

   }

   hostInput = argv[1];
   clientPort = atoi(argv[2]);
   ipVal = inet_pton(AF_INET, hostInput,& ip);//host ip validation

   if(ipVal != 1){
       printf("%s is an invalid IPv4 address.\nTry again.\n",hostInput);
       return 0;
   }
   if((clientPort > 65535) || (clientPort < 0) ){
      printf("Client port exceeds 16-bit range\nTry again\n");
      return 0;
   }
  //    else if(!isdigit(clientPort)){
  //    printf("Invalid port input\nTry again\n");
  //    return 0;
  // }

   host = (struct hostent *) gethostbyname((char *)hostInput);
   if(host == NULL){

       printf("Host %s does not exist\n", hostInput);
       return 0;
   }

//socket(int domain, int type, int protocol)

   if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

      perror("socket");
      exit(1);
   }

   server_addr.sin_family = AF_INET;

   server_addr.sin_port = htons(clientPort);

   server_addr.sin_addr = *((struct in_addr *)host->h_addr);

   bzero(&(server_addr.sin_zero),8);

   bcopy((char *)host->h_addr, (char *)&server_addr.sin_addr.s_addr, host->h_length);

while(1){
        printf("Type some data to send (q or Q to quit):");

        fgets(send_data, sizeof(send_data), stdin); //Gets data from user input and stores it in send_data

        if (strcmp(send_data , "q\n") == 0 || strcmp(send_data ,"Q\n") == 0)//Checks that quit command has not been inputted
        {
          sprintf(quit,"QUITTING ON USER REQUEST\n");
          sendto(sock, quit, strlen(quit),0,
              (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
          close(sock);
          return 0;
        }
        else{
          sendto(sock, send_data, strlen(send_data), 0,
          (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
        }
    }
}
