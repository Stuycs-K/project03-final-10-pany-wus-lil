#include "networking.h"
#include <ctype.h>

void clientLogic(int server_socket) {
    printf("Write a string to convert: ");
    //char* data = calloc(100,sizeof(char));
    //fgets(data,100,stdin);
    char* data = "testing";
    printf("Client attemping to write to server\n");
    write(server_socket,data,strlen(data));
    printf("Client wrote to server\n");
    printf("Client attempting to read server\n");
    read(server_socket,data,100);
    //data[strlen(data)-1] = ' ';
    printf("Client read: %s\n",data);
}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  clientLogic(server_socket);
  close(server_socket);
}