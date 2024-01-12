# Dev Log:

## Yuchen Pan

### 2024-01-03 - Proposal
Finished the description of the project and how we adjusted the uno game rules to implement our project

### 2024-01-04 - Server Setup
Set up server that allow multiple clients to connect, the for loop add the client if it's trying to connect and wait until the all the clients are connected.

### 2024-01-05 - Signal
Added sighandler to control client quitting, exits the loop when client use ctrl c or when client disconnects, file descriptor is removed

### 2024-01-08 - Signal



## Lisa Li

### 2024-01-04 - Create cards
Able to create a specific card provided that the color and number is given or print n number of random cards. A print function was made to allow for easier viewing. I was not able to create the cards in the random method (there's a bus error I can't seem to resolve). ~ 1 hour.

### 2024-01-05 - Changing to Linked List
Original method kept leading to bus error and segmentation faults, so I decided to integrate linked lists into the card struct. I added a next so that I link it to the next card. Might add a prev to do a doubly linked list. Created a print cards function printing the cards and the total. ~ 1 hour

### 2024-01-08 - Create hand of cards + random
After switching to a singly linked list, I coded methods to add a card to the end of a list, as well as print out the entire list, noting how many cards are in the list total. I've begun working on the beginning distribution of random cards as well as testing for any possible loopholes. I've create a method to produce a list of n number of random cards using srand(time). ~ 1.1 hour

### 2024-01-09 - Remove card + draw
I wrote a method to remove a card from the list, works for first, middle, and last nodes. I also created a draw card method that will add a random card to the end of the list. Made search method to enhance the remove method (whether or not you can play the card). ~ 1.1 hours

### 2024-01-10 - Completed card methods -> merging
After extensive testing of all the card methods, I've started to merge these methods with the client and server. This includes putting the methods into a header file that both the client and the server will include. I wanted the server to make and send the initial 7 cards to the client, but after a long struggle, I was unable to send the list of cards through the socket to the client. ~ 1.2 hours

### 2024-01-11 - Distribute cards + remove
Since I was unable to send the list of cards through the socket to the client, I decided to just let the client make 7 random cards (BUG: cards appear after joining immediately). In the same manner, I decided to let the client remove/play the card they want. If the client cannot play the card (because they don't have it), they are prompted over and over until they enter a card that they do have. ~ 1.5 hours


## Sherlyn Wu

### 2024-01-03 - Proposal
Worked on what Uno features would be implemented and how

### 2024-01-04 - Client setup
Set up basic client

### 2024-01-05 - Client-Server behavior
Client is able to connect to server
Server prints message when all three clients are connected and write to it
~ 1 hour

### 2024-01-08 - Turns
Server handles which client's turn it is
Client has different behavior depending on if it's the client's turn or not
Bug: sometimes client's turn functions normally, sometimes does not
Removed sleep calls and clients seem to be working, more thorough testing will be conducted tomorrow
Added infrastructure (debug print, sighandler, when server dies the clients die gracefully with it)
~ 2 hours

### 2024-01-09 - Rounds
Server rereads from clients after round ends, allowing for multiple rounds
Deck established; last client's card is stored in the top of the deck in preparation for the next client to use
More advanced debugging print function
Attempted to debug: client sometimes reads card on deck and isturn in a single read, blocking the consecutive read
~ 2 hours
