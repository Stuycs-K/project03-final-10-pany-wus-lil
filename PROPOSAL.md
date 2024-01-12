# Final Project Proposal

## Group Members:

Yuchen Pan, Sherlyn Wu, Lisa Li
       
# Intentions:

Uno card game

For at least two players, the server distributes 7 cards to each player. 

Cards are displayed to the players in texts. (Ex: r4, b7)

Order will be randomly selected. 

Player 1 takes the first turn, player 2 takes the second turn, etc


Turn ends when:

the client plays a card

the client draws at least one card and types “end”

the client has 0 cards in their hand


Error when:

play a card that is invalid or does not exist

play multiple cards at the same time

play a different card from the card the player drew


When someone has 1 card, UNO is announced to all players


The game ends when: 

a player has 0 cards


After a player wins, the server tells all clients who won

Shut down the server and attach clients

    
# Intended usage:

A description as to how the project will be used (describe the user interface).
  
# Technical Details:

KEY CONCEPTS: 

Semaphores will be used to only allow a certain number of clients (requested by the server) to join/play the game at any given time. 

The server will fork/create subservers that will individually communicate with each client regarding the cards on hand and what the client chooses to play.

Signals will be used to allow clients to prematurely quit the game. Game will end when one of the client quits or disconnects.

Sockets will allow for clients to communicate to the (sub)server what card they would like play, if they want to draw a card, and if they want to end their turn. 

Files/Shared Memory might be used to communicate the last played card (or the starting card) or the entire play history to all the clients.

     
DATA STRUCTURES/ALGORITHMS:

We will use lexcial sorting to sort the clients' hands by color (this should occur at the beginning when cards are dealt).

We will probably use either singly or doubly linked lists to implement the sorting and display of the hand (cards are structs). This will make removing a card at the center of the list much easier as linked lists are dynamic. When a new card is added, it'll insert by color (but the numbers will out of order).
- could also replace that with arrays

Stacks could be used to store the play history of the match. This would mean that the most recently played card will be at the top of the stack (can use peek()). 


WHO DOES WHAT:
Breakdown: 
Server Setup (# of players, 7 cards each, set order of turns)
Client (play a card, draw a card, end turn)
(sub)Server (read from client, write what was played, next player)

Yuchen - server setup, signals/premature client quitting, sockets, discarded pile

Sherlyn - client/turn implementation, card drawing, game end

Lisa - create deck of cards or creating random cards, randomly create lists of 7 cards, semaphore implementation, lexical sorting at beginning.


# Intended pacing:
Start: Jan 4 class
Jan 4/5 - simple server setup, simple client program, card deck, semaphore
Jan 8/9/10 - deal cards to clients, sockets, discarded pile (show last played)
Jan 11/12 - draw card and play?, uno, signals, display winner
Jan 14/15 - testing, rehearsing for presentation, make sure to test on lab machine
End: Jan 16 8am

