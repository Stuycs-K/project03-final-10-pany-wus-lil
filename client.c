#include "networking.h"
#include <ctype.h>

void clientLogic(int server_socket) {
    char* data = calloc(100,sizeof(char));
    /**
    printf("Enter card you want to play: ");
    fgets(data,100,stdin);
    printf("\n");
    write(server_socket,data,strlen(data));**/
    read(server_socket,data,8);
    // c'est un trimmer
    // data[strlen(data)-1] = ' ';
}

int clienthandshake(char* server_address) {
  char* PORT = "9998";
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  getaddrinfo(server_address, PORT, hints, &results);

  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  
  //connect to the server
  connect(serverd,results->ai_addr,results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);
  return serverd;
}

int main(int argc, char *argv[] ) {
  char* ID = "127.0.0.1";
  if(argc>1){
    ID=argv[1];
  }
  
  int server_socket = clienthandshake(ID);

  //printf("client connected.\n");
  clientLogic(server_socket);

  close(server_socket);
}