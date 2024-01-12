#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include "networking.h"

#define MAX_CLIENTS 3

/**
TODO LIST
-Encapsulate
-fork server so it's not stuck handling all 3 clients in order
-move DEBUG print to a more suitable file

TEMPORARY MEASURES

**/

static void sighandler (int signo) {
    if (signo == SIGINT) {
        //printf("we are in the sighandler\n");
        exit(0);
    }
}

bool search(struct card * head, char _color, int num){
  struct card * current = head;
  while (current){
    if (current->color == _color && current->number == num){
      return true;
    }
    current = current->next;
  }
  return false;
}

bool removeCard(struct card ** head, char _color, int num){
  struct card * temp;
  struct card * current = * head;
  if (search(* head, _color, num)){
    if (current && current->color == _color && current->number == num) { // removing first node
      temp = current;
      current = current->next;
      free(temp);
      * head = current;
      return true;
    } else {
      while (current){
        if (current->next != NULL && current->next->color == _color && current->next->number == num){
          temp = current->next;
          current->next = temp->next;
          free(temp);
        }
        current = current->next;
      }
      return true;
    }
  } else return false;
}

struct card * makeHand(int n){
  srand(time(NULL));
  struct card * head = (struct card *) malloc(sizeof(struct card));
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  while (n > 0){
    cC = rand() % 4;
    cN = rand() % 10;
    //printf("color: %c\nnumber: %d\n", cardColor[cC], cardNumber[cN]);
    //printf("card: %c%d\n", cardColor[cC], cardNumber[cN]);
    add(head, cardColor[cC], cardNumber[cN]);
    //printCard(tmp);
    n--;
  }
  removeCard(&head, 0, 0);
  return head;
}


/*void printCard(struct card * _card){
  char info[3];
  char cardColor = _card->color;
  info[0] = cardColor;
  //printf("info[0]: %c\n", info[0]);
  info[1] = _card->number + '0';
  //printf("info[1]: %c\n", info[1]);
  info[2] = '\0';
  //printf("info[2]: %c\n", info[2]);
  char * cardData = info;
  //printf("info: %s\n", info);
  printf("your card is: %s\n", cardData);
}*/

char* clientTurn(int client_socket, char* isturn_y, char*buff, int i) {
    DEBUG("server attempting to write to client\n");
    write(client_socket, isturn_y, strlen(isturn_y));
    DEBUG("server successfully wrote to client\n");
    //read the whole thing
    DEBUG("server attempting to read from client\n");
    read(client_socket, buff, sizeof(buff) - 1);
    //trim
    buff[strlen(buff) - 1] = '\0';
    if  (buff[strlen(buff) - 1] == 13) {
        buff[strlen(buff) - 1] = '\0';
    }
    printf("Received from client %d: '%s'\n", i + 1, buff);
    return buff;
}

int main() {
    signal(SIGINT,sighandler);

    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    char* PORT = "9998";
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; // TCP socket
    hints->ai_flags = AI_PASSIVE; // only needed on server
    getaddrinfo(NULL, PORT, hints, &results); // NULL is localhost or 127.0.0.1

    //create socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    //free port after program exit
    int yes = 1;
    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("sockopt  error\n");
        printf("%s\n", strerror(errno));
        exit(-1);
    }

    int err = bind(listen_socket, results->ai_addr, results->ai_addrlen);
    if(err == -1){
        printf("Error binding: %s", strerror(errno));
        exit(1);
    }
    listen(listen_socket, MAX_CLIENTS); // Maximum # of clients that can connect
    printf("Listening on port %s\n", PORT);

    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    fd_set read_fds;
    int client_sockets[MAX_CLIENTS] = {0}; // Array to store client sockets

    while (1) {

        DEBUG("start of the while loop\n");

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(listen_socket, &read_fds);

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_sockets[i] > 0) {
                FD_SET(client_sockets[i], &read_fds);
            }
        }

        int max_socket = listen_socket + 1;
        int i = select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

        if (i == -1) {
            perror("Error in select");
            exit(-1);
        }

        if (FD_ISSET(listen_socket, &read_fds)) {
            int client_socket = accept(listen_socket, (struct sockaddr*)&client_address, &sock_size);
            if (client_socket > 0) {
                // Add the client if there's a client trying to connect
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (client_sockets[i] == 0) {
                        client_sockets[i] = client_socket;
                        break;
                    }
                }
                //printf("Client connected.\n"); uncomment when done DEBUGging
                printf("Client %d connected.\n",client_socket);
            }
        }

        //wait for 3 (max num) clients
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_sockets[i] > 0 && FD_ISSET(client_sockets[i], &read_fds)) {
                char buff[1025] = "";
                //read the whole thing
                read(client_sockets[i], buff, sizeof(buff) - 1);
                //trim
                buff[strlen(buff) - 1] = '\0';
                if (buff[strlen(buff) - 1] == 13) {
                    buff[strlen(buff) - 1] = '\0';
                }
                printf("Received from client %d: '%s'\n", i + 1, buff);
                //handle clinet message later here
            }
        }

        // code to test a basic turn
        if (client_sockets[2] != 0) {
            printf("All 3 clients have connected.\n");

            // temporary variable to store the card on top of the deck
            //char* toppadeck = calloc(100,sizeof(char));
            //toppadeck = "soy first card";

            // enter the main loop of the game - put this into a separate function
            for (int i = 0; i < MAX_CLIENTS; i++){
              char count[256];
              sprintf(count, "Your 7 cards are: \n%d", 7);
              write(client_sockets[i], count, sizeof(count), 0);

              // Then send that many node's worth of name-data
              struct struct * n = headNode;
              while(n)
              {
                send(sfd, n->name, sizeof(n->name), 0);
                n = n->next;
              }
            }

            char * clientCards = calloc(100,sizeof(char));
            for (int i = 0; i < MAX_CLIENTS; i++){
              struct card * cards = makeHand(7);
              fgets(data,100,printCards(cards));
              write(client_sockets[i], cards, sizeof(cards));
            }
            while(1) {
              for (int i = 0; i < MAX_CLIENTS; i++) {
                /**
                plan: For every cycle of the loop, i is the client whose turn it is
                all clients read isturn from the server
                if isturn, that client writes its card to the server
                if !isturn, that client doesn't do anything
                **/
                char* isturn_y = "y";
                char* isturn_n = "n";
                char buff[1025] = "";
                    //printf("Card on deck: %s\n",toppadeck);
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        // writes card on deck to clients
                        //write(client_sockets[j], toppadeck, strlen(toppadeck));
                        if (j == i) {
                            //strcpy(toppadeck,clientTurn(client_sockets[j],isturn_y,buff,i));
                            clientTurn(client_sockets[j],isturn_y,buff,i);
                        } else {
                            write(client_sockets[j], isturn_n, strlen(isturn_n));
                        }
                    }
                }
                printf("Round over\n");
            }

        }
    }

    printf("server killed\n");

    // Close client sockets
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] > 0) {
            close(client_sockets[i]);
        }
    }

    free(hints);
    freeaddrinfo(results);


    /*struct card * head = create('r', 9);
    printf("adding a card r9\n");
    printf("adding a card y0\n");
    add(head, 'y', 0);
    printCards(head);
    printf("adding a card g5\n");
    add(head, 'g', 5);
    printCards(head);

    printf("searching for r3\n");
    if (search(head, 'r', 3)){
      printf("r3 found\n");
    } else printf("r3 not found\n");
    printf("searching for r9\n");
    if (search(head, 'r', 9)){
      printf("r9 found\n");
    } else printf("r9 not found\n");

    printf("removing a card g0\n");
    if (removeCard(&head, 'g', 0)){
      printf("g0 removed\n");
    } else printf("g0 not removed\n");
    printCards(head);
    printf("removing a card y0\n");
    if (removeCard(&head, 'y', 0)){
      printf("y0 removed\n");
    } else printf("y0 not removed\n");
    printCards(head);
    printf("removing a card r9\n");
    removeCard(&head, 'r', 9);
    printCards(head);
    printf("removing a card g5\n");
    removeCard(&head, 'g', 5);
    printCards(head);


    printf("\nnew list\n\n");
    struct card * hand = makeHand(7);
    printCards(hand);
    printf("adding a card g5\n");
    add(hand, 'g', 5);
    printCards(hand);
    printf("drawing a card\n");
    draw(hand);
    printCards(hand);*/

    return 0;


}
