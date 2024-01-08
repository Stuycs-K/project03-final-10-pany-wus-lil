# Dev Log:

## Yuchen Pan

### 2024-01-03 - Proposal
Finished the description of the project and how we adjusted the uno game rules to implement our project

### 2024-01-04 - Server Setup
Set up the server that allows multiple clients to connect, the for loop add the client if it's trying to connect, and wait until all the clients are connected.

### 2024-01-05 - Signals
Use signals to handle client quitting using ctrl c or quitting by disconnecting - when byte = 0, fd is removed

## Lisa Li

### 2024-01-04 - Create cards
Able to create a specific card provided that the color and number is given or print n number of random cards. A print function was made to allow for easier viewing. I was not able to create the cards in the random method (there's a bus error I can't seem to resolve). ~ 1 hour.

### 2024-01-05 - Changing to Linked List
Original method kept leading to bus error and segmentation faults, so I decided to integrate linked lists into the card struct. I added a next so that I link it to the next card. Might add a prev to do a doubly linked list. Created a print cards function printing the cards and the total. ~ 1 hour
