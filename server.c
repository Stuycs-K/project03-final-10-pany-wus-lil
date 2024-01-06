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

#define MAX_CLIENTS 3

struct card {
  char color;
  int number;
  struct card *next;
};

struct card * create(char _color, int _number){
  struct card * tmp;
  tmp->color = _color;
  tmp->number = _number;
  return tmp;
}

void printCards(struct card * hand) {
  printf("printing cards\n");
  int count = 0;
  while (hand != NULL) {
    printf("%c%d\n", hand->color, hand->number);
    count++;
    hand = hand->next;
  }
  printf("total: %d cards\n", count);

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

int main() {
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
                printf("Client connected.\n");
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
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    // if isturn
                    if (j == i) {
                        write(client_sockets[j], isturn_y, strlen(isturn_y));
                    } else {
                        write(client_sockets[j], isturn_n, strlen(isturn_n));
                    }
                }

                /**
                //read the whole thing
                read(client_sockets[i], buff, sizeof(buff) - 1);
                //trim
                buff[strlen(buff) - 1] = '\0';
                if (buff[strlen(buff) - 1] == 13) {
                    buff[strlen(buff) - 1] = '\0';
                }
                printf("Received from client %d: '%s'\n", i + 1, buff);**/
            }
        }
    }

    // Close client sockets
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] > 0) {
            close(client_sockets[i]);
        }
    }

    free(hints);
    freeaddrinfo(results);

    struct card * a;
    a->color = 'r';
    a->number = 3;
    a->next = NULL;

    struct card * b;
    b->color = 'y';
    b->number = 9;
    a->next = b;
    printCards(a);
    //a->next = b;
    //printCards(a);
    //printf("creating 7 random cards\n");
    //randomCard(7);
    //printf("drawing a card\n");
    //drawCard();

    return 0;


}