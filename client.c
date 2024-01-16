#include "networking.h"
#include <ctype.h>

void clientIsKil() {
  printf("\e[1mYou have served your purpose. All that awaits you now is the gift of death. The darkness beyond your final days.\e[m");
  exit(-1);
}

void clientLogic(int server_socket) {
  // ****** CARDS APPEAR AS SOON AS THE CLIENT CONNECTS *********
  // reads card on deck
  /*DEBUG("client attempting to read card on deck\n");
  read(server_socket,data,100);
  printf("Card on deck: %s\n",data);*/

  struct card * hand = makeHand(7);
  printf("Cards in hand:\n");
  printCards(hand);

  while (1) {

    //sleep(1); // prevents spam
    char* data = calloc(100,sizeof(char));

    // COD code here (MAKE SURE TO COMMENT OUT IF DOES NOT WORK)
    DEBUG("client attempting to read card on deck\n");
    // replace sizeof(char)*2 with size of struct card
    int cod_result = read(server_socket,data,sizeof(char)*2);
    DEBUG("cod read result: %d\n", cod_result);
    // client only needs to know what toppadeck is if it's the client's turn
    char* toppadeck = calloc(100,sizeof(char));
    strcpy(toppadeck,data);
    // COD code ends here

    // receives isturn
    DEBUG("client is trying to read\n");
    int read_result = read(server_socket,data,sizeof(char));
    DEBUG("read result: %d\n", read_result);
    DEBUG("data read: %s\n",data);

    // if read is unsuccessful (server is dead), kill
    if (read_result != 1) {
      break;
    }

    // if isturn
    //printf("received from server: %s\n", data);
    if (data[0] == 'y') {
      bool playable, canPlay;
      printf("It is now your turn.\n");
      printf("Card on deck: %s\n",toppadeck);
      printf("Enter card you want to play: ");
      fgets(data,100,stdin);
      //printf("color: %c\n", toppadeck[0]);
      //printf("number: %d\n", toppadeck[1] - '0');
      if (data[0] == 'd'){
        printf("drawing a card\n");
        draw(hand);
        printCards(hand);
        printf("Enter card you want to play: ");
        fgets(data,100,stdin);
        playable = removeCard(&hand, data[0], data[1] - '0');
        canPlay = matches(toppadeck[0], toppadeck[1] - '0', data[0], data[1] - '0');
      } else {
        playable = removeCard(&hand, data[0], data[1] - '0');
        canPlay = matches(toppadeck[0], toppadeck[1] - '0', data[0], data[1] - '0');
      }
      while (canPlay == false){
        printf("Card doesn't match.\n\n");
        printf("Enter card you want to play: ");
        fgets(data,100,stdin);
        canPlay = matches(toppadeck[0], toppadeck[1] - '0', data[0], data[1] - '0');
      }
      while (playable == false){
        printf("You don't have this card.\n\n");
        printf("Enter card you want to play: ");
        fgets(data,100,stdin);
        playable = removeCard(&hand, data[0], data[1] - '0');
      }
      printf("\nCards in hand:\n");
      int numberOfCards = printCards(hand);
      write(server_socket,data,strlen(data));
      if (numberOfCards == 1) {
        printf("\e[1mUNO!\e[m\n");
      } else if (numberOfCards == 0) {
        printf("\e[1mGAME OVER!\e[m\n");
      }
    } else if (data[0] == 'n') {
      // if not your turn
      DEBUG("Not the client's turn\n");
    } else {
      clientIsKil();
    }

    //DEBUG("isturn split over\n");
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

  DEBUG("client closing due to natural causes\n");
  close(server_socket);
}
