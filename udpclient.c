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

int main(int argc, char *argv[]) {

int sock;
int clientPort;

struct sockaddr_in server_addr;
struct hostent *host;

char send_data[1024];   

//check that correct number of arguments are given
   
if(argc != 3){
    
    fprintf(stderr,"Incorrect number of arguments\nUsage: udpclient [host ip] [client port]\n");
    exit(1);
   
}

//host = (struct hostent *) gethostbyname((char *)"127.0.0.1");

// Get Host input and check if it is valid
host = gethostbyname(argv[1]);
if(host == NULL){
    perror("Invalid Host IP");
    exit(1);
}

//Get Port and check if it is valid
clientPort = atoi(argv[2]);
if(clientPort > 65535)
{
    perror("Client port exceeds 16-bit range\n")     
    exit(1);
}

//socket(int domain, int type, int protocol)   
if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
      
    perror("socket");
    exit(1);
}

   
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(clientPort);
server_addr.sin_addr = *((struct in_addr *)host->h_addr);
bzero(&(server_addr.sin_zero),8);
   
while (1) 
{

    printf("Type some data to send (q or Q to quit):");
      
    fgets(send_data, sizeof(send_data), stdin); //Gets data from user input and stores it in send_data
       
    if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)//Checks that quit command has not been inputted
       	          break;

    else{
          
        printf("QUITTING ON USER REQUEST\n");
          
        sendto(sock, send_data, strlen(send_data), 0,
              
            (struct sockaddr *)&server_addr, sizeof(struct sockaddr));   
    }
}
     
   

}
   
}
