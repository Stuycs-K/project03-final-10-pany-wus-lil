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
#include <semaphore.h>
#include "networking.h"

#define MAX_CLIENTS 3

sem_t lastCardSemaphore;  // Semaphore for last card played
sem_t historySemaphore;   // Semaphore for card played history

/**
TODO LIST
- Encapsulate
- Fork server so it's not stuck handling all 3 clients in order
- Move DEBUG print to a more suitable file

TEMPORARY MEASURES

**/

// Signal handling function
static void sighandler(int signo) {
    if (signo == SIGINT) {
        sem_destroy(&lastCardSemaphore);
        sem_destroy(&historySemaphore);
        exit(0);
    }
}

// Function to simulate client's turn
char* clientTurn(int client_socket, char* isturn_y, char* buff, int i) {
    DEBUG("server attempting to write to client\n");
    write(client_socket, isturn_y, strlen(isturn_y));
    DEBUG("server successfully wrote to client\n");

    // Simulate client's card play
    // In a real scenario, read the actual card played from the client
    char simulatedCard[1025] = "Simulated Card";
    sem_wait(&lastCardSemaphore);
    printf("Last Card Played: %s\n", simulatedCard);
    sem_post(&lastCardSemaphore);

    // Simulate history update
    sem_wait(&historySemaphore);
    printf("Updating Card History...\n");
    // Update the card history here
    sem_post(&historySemaphore);

    return buff;
}

int main() {
    signal(SIGINT, sighandler);

    // Initialize semaphores
    sem_init(&lastCardSemaphore, 0, 1);  // Initial value of 1 (mutex)
    sem_init(&historySemaphore, 0, 1);   // Initial value of 1 (mutex)

    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    char* PORT = "9998";
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;  // TCP socket
    hints->ai_flags = AI_PASSIVE;      // only needed on the server
    getaddrinfo(NULL, PORT, hints, &results);  // NULL is localhost or 127.0.0.1

    // Create socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    // Free port after program exit
    int yes = 1;
    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("sockopt  error\n");
        printf("%s\n", strerror(errno));
        exit(-1);
    }

    int err = bind(listen_socket, results->ai_addr, results->ai_addrlen);
    if (err == -1) {
        printf("Error binding: %s", strerror(errno));
        exit(1);
    }
    listen(listen_socket, MAX_CLIENTS);  // Maximum # of clients that can connect
    printf("Listening on port %s\n", PORT);

    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    fd_set read_fds;
    int client_sockets[MAX_CLIENTS] = {0};  // Array to store client sockets

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
                // printf("Client connected.\n"); uncomment when done DEBUGging
                printf("Client %d connected.\n", client_socket);
            }
        }

        // Wait for 3 (max num) clients
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_sockets[i] > 0 && FD_ISSET(client_sockets[i], &read_fds)) {
                char buff[1025] = "";
                // Read the whole thing
                read(client_sockets[i], buff, sizeof(buff) - 1);
                // Trim
                buff[strlen(buff) - 1] = '\0';
                if (buff[strlen(buff) - 1] == 13) {
                    buff[strlen(buff) - 1] = '\0';
                }
                printf("Received from client %d: '%s'\n", i + 1, buff);
                // Handle client message later here
            }
        }

        // Code to test a basic turn
        if (client_sockets[2] != 0) {
            printf("All 3 clients have connected.\n");

            while (1) {
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
                        // Writes card on deck to clients
                        // Write the last card played to the clients
                        sem_wait(&lastCardSemaphore);
                        write(client_sockets[j], simulatedCard, strlen(simulatedCard));
                        sem_post(&lastCardSemaphore);

                        if (j == i) {
                            clientTurn(client_sockets[j], isturn_y, buff, i);
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

    // Destroy semaphores
    sem_destroy(&lastCardSemaphore);
    sem_destroy(&historySemaphore);

    free(hints);
    freeaddrinfo(results);

    return 0;
}
