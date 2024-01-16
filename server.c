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

static void sighandler (int signo) {
    if (signo == SIGINT) {
        //printf("we are in the sighandler\n");
        exit(0);
    }
}

char* clientTurn(int client_socket, char* isturn_y, char*buff, int i) {
    DEBUG("server attempting to write to client\n");
    write(client_socket, isturn_y, strlen(isturn_y));
    DEBUG("server successfully wrote to client\n");
    //read the whole thing
    DEBUG("server attempting to read from client\n");
    read(client_socket, buff, sizeof(buff) - 1);
    //trim
    // trim has been moved to client 
    /**
    buff[strlen(buff) - 1] = '\0';
    if  (buff[strlen(buff) - 1] == 13) {
        buff[strlen(buff) - 1] = '\0';
    }**/
    printf("%s\n",buff);
    DEBUG("Received from client %d: '%s'\n", i + 1, buff);
    return buff;
}

int main() {
    signal(SIGINT,sighandler);

    int listen_socket = server_setup();

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
                    For every cycle of the loop, i is the client whose turn it is
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

                            // adds toppadeck
                            toppadeck = calloc(strlen(temp),sizeof(char));
                            char* moveToToppa = calloc(10,sizeof(char));
                            moveToToppa = strtok(temp, ",");
                            strcpy(toppadeck,moveToToppa);
                            DEBUG("toppadeck shoudl just be a card: %s\n",toppadeck);

                            // end of turn conditions: 
                            // w for victory/game end, n for proceeding as normal
                            //printf("\e[1mGAME OVER!\e[m\n");
                            //char* data = calloc(10,sizeof(char));
                            //strcpy(data,temp);
                            //read(client_sockets[j],data,sizeof(char));
                            //printf("received gameover from client? %s",data);
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

    return 0;

}
