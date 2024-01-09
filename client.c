#include "networking.h"
#include <ctype.h>

void clientLogic(int server_socket) {
  while (1) {
    //sleep(1); // prevents spam
    char* data = calloc(100,sizeof(char));
    debug("client is trying to read\n");
    int read_result = read(server_socket,data,100);
    printf("read result: %d\n", read_result);
    
    // if read is unsuccessful (server is dead), kill
    if (read_result != 1) {
      break;
    }

    // if isturn
    //printf("received from server: %s\n", data);
    if (strcmp(data,"y") == 0) {
      printf("Enter card you want to play: ");
      fgets(data,100,stdin);
      write(server_socket,data,strlen(data));
    } else {
      // if not your turn
      printf("It is not your turn.\n");
    }
  }
    /**
    char* data = calloc(100,sizeof(char));
    fgets(data,100,stdin);
    printf("Client attemping to write to server\n");
    write(server_socket,data,strlen(data));
    printf("Client wrote to server\n");
    printf("Client attempting to read server\n");
    read(server_socket,data,8);
    // c'est un trimmer
    // data[strlen(data)-1] = ' ';**/
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

  debug("client closing due to natural causes\n");
  close(server_socket);
}