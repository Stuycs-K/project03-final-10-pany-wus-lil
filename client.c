#include "networking.h"
#include <ctype.h>

void clientIsKil() {
    printf("\e[1mYou have served your purpose. All that awaits you now is the gift of death. The darkness beyond your final days.\e[m\n");
    exit(-1);
}

void clientLogic(int server_socket) {
    struct card *hand = makeHand(7);
    printf("Cards in hand:\n");
    printCards(hand);

    while (1) {
        char data[BUFFER_SIZE];
        memset(data, 0, sizeof(data));

        // Read the response from the server
        int read_result = read(server_socket, data, BUFFER_SIZE);

        // Check if the response is an error message
        if (strncmp(data, "Error:", 6) == 0) {
            printf("Server says: %s\n", data);
            continue; // Continue to the next iteration of the loop
        }

        // Check if the client requested the played cards history
        if (strncmp(data, "history", 7) == 0) {
            printf("Played cards history:\n%s", data);
            continue; // Continue to the next iteration of the loop
        }

        // Check if the client's turn
        if (data[0] == 'y') {
            printf("It is now your turn.\n");
            printf("Card on deck: %s\n", data + 1);

            // Enter card you want to play
            printf("Enter card you want to play (e.g., 'ry' for red  yellow): ");
            fgets(data, BUFFER_SIZE, stdin);
            write(server_socket, data, strlen(data));

            // Print updated cards in hand
            printf("Cards in hand:\n");
            printCards(hand);

            // Check if UNO or GAME OVER
            if (strstr(data, "UNO") != NULL) {
                printf("\e[1mUNO!\e[m\n");
            } else if (strstr(data, "GAME OVER") != NULL) {
                printf("\e[1mGAME OVER!\e[m\n");
                break; // Exit the loop when the game is over
            }
        } else if (data[0] == 'n') {
            // Not the client's turn
            printf("Not the client's turn\n");
        } else {
            // Server closed or unexpected response
            clientIsKil();
        }
    }
}

int clienthandshake(char *server_address) {
    char *PORT = "9998";
    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; // TCP socket
    getaddrinfo(server_address, PORT, hints, &results);

    int serverd; // Store the socket descriptor here
    // Create the socket
    serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    // Connect to the server
    connect(serverd, results->ai_addr, results->ai_addrlen);

    free(hints);
    freeaddrinfo(results);
    return serverd;
}

int main(int argc, char *argv[]) {
    char *ID = "127.0.0.1";
    if (argc > 1) {
        ID = argv[1];
    }

    int server_socket = clienthandshake(ID);

    printf("client connected.\n");
    clientLogic(server_socket);

    DEBUG("client closing due to natural causes\n");
    close(server_socket);

    return 0;
}
