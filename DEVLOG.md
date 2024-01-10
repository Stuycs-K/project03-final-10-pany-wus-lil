# Dev Log:

## Yuchen Pan

### 2024-01-03 - Proposal
Finished the description of the project and how we adjusted the uno game rules to implement our project

### 2024-01-04 - Server Setup
Set up server that allow multiple clients to connect, the for loop add the client if it's trying to connect and wait until the all the clients are connected. 

## Lisa Li

### 2024-01-04 - Create cards
Able to create a specific card provided that the color and number is given or print n number of random cards. A print function was made to allow for easier viewing. I was not able to create the cards in the random method (there's a bus error I can't seem to resolve). ~ 1 hour.

### 2024-01-03 - Brief description
Expanded description including how much time was spent on task.

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