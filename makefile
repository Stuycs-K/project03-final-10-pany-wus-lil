compile: clienta servera
clienta: client.o networking.o
	@gcc -o client client.o networking.o
servera: server.o networking.o
	@gcc -o server server.o networking.o
client.o: client.c networking.h
	@gcc -c client.c
server.o: server.c networking.h
	@gcc -c server.c
client: clienta
	@./client
server: servera
	@./server
networking.o: networking.c networking.h
	@gcc -c networking.c
clean:
	@rm *.o server client *~
