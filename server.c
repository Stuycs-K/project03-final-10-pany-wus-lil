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

struct card {
  char color;
  int number;
  struct card *next;
};

struct card * create(char _color, int num){
  struct card * tmp = (struct card *) malloc(sizeof(struct card));
  tmp->color = _color;
  tmp->number = num;
  tmp->next = NULL;
  return tmp;
}

void add(struct card * head, char _color, int num){
  struct card * temp;
  struct card * new = create(_color, num);
  temp = head;
  while(temp != NULL && temp->next != NULL){
    temp = temp->next;
  }
  temp->next = new;
}

struct card * makeHand(int n){
  srand(time(NULL));
  struct card * head = (struct card *) malloc(sizeof(struct card));
  struct card * tmp;
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  while (n > 1){
    cC = rand() % 4;
    cN = rand() % 10;
    //printf("color: %c\nnumber: %d\n", cardColor[cC], cardNumber[cN]);
    //printf("card: %c%d\n", cardColor[cC], cardNumber[cN]);
    add(tmp, cardColor[cC], cardNumber[cN]);
    //printCard(tmp);
    n--;
  }
  head = tmp;
  return head;
}

/**
struct card * remove(struct card * head, char _color, int num){
  if (head == NULL){
    return NULL;
  }
  if (head->color == _color && head->number == num) {
    struct card * temp;
    temp = head->next;
    free(head);
    return temp;
  }
  head->next = remove(head->next, _color, num);
  return head;
}**/

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



void printCards(struct card * hand) {
  printf("printing cards\n");
  int count = 0;
  while (hand != NULL) {
    printf("%c%d\n", hand->color, hand->number);
    count++;
    hand = hand->next;
  }
  printf("total: %d cards\n\n", count);

}

void printCard(struct card * _card){
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
}

void randomCard(int n){
  srand(time(NULL));
  char cardColor[4] = {'r', 'y', 'g', 'b'};
  int cardNumber[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int cC, cN;
  rand();
  while (n > 0){
    cC = rand() % 4;
    cN = rand() % 10;
    //printf("color: %c\nnumber: %d\n", cardColor[cC], cardNumber[cN]);
    printf("card: %c%d\n", cardColor[cC], cardNumber[cN]);
    //struct card * tmp = create(cardColor[cC], cardNumber[cN]);
    //printCard(tmp);
    n--;
  }
}

void drawCard(){
  randomCard(1);
}

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
    printf("%s\n",buff);
    printf("Received from client %d: '%s'\n", i + 1, buff);
    return buff;
}

int main() {
    signal(SIGINT,sighandler);

    printf("(remove when cards are being written/read from client instead of strings)\n");
    printf("ONLY send strings with a length of 2 or the client has a stroke!!!!!!!!!\n");

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
                if (buff == NULL) {
                    printf("Closing server due to disconnected client\n");
                    exit(-1);
                }
                //handle clinet message later here
            }
        }

        // code to test a basic turn
        if (client_sockets[2] != 0) {
            printf("All 3 clients have connected.\n");

            // temporary variable to store the card on top of the deck
            char* toppadeck = calloc(100,sizeof(char));
            toppadeck = "00";

            // enter the main loop of the game - put this into a separate function
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
                    printf("Card on deck: %s\n",toppadeck);
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        // COD code here (MAKE SURE TO COMMENT OUT IF DOES NOT WORK)
                        DEBUG("server trying to write cod\n");
                        write(client_sockets[j], toppadeck, strlen(toppadeck));
                        DEBUG("server wrote cod\n");
                        // COD code ends here
                        if (j == i) {
                            char* temp = clientTurn(client_sockets[j],isturn_y,buff,i);
                            DEBUG("clientTurn result: %s\n",temp);
                            toppadeck = calloc(strlen(temp),sizeof(char));
                            strcpy(toppadeck,temp);
                            //clientTurn(client_sockets[j],isturn_y,buff,i);
                        } else {
                            DEBUG("Server writing isturn_n to client %d\n",j);
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
/*
    struct card * head = create('r', 9);
    add(head, 'y', 0);
    printCards(head);
    add(head, 'g', 5);
    printCards(head);
    struct card * hand = makeHand(7);
    printCards(hand);
    remove(head, 'y', 0);
    printCards(head); */
    /*struct card * b = create('y', 0);


    struct card * a;
    a->color = 'r';
    a->number = 3;
    a->next = NULL;

    struct card * b;
    b->color = 'y';
    b->number = 9;
    a->next = b;
    printCards(a);**/
    //a->next = b;
    //printCards(a);
    //printf("creating 7 random cards\n");
    //randomCard(7);
    //printf("drawing a card\n");
    //drawCard();

    return 0;


}
