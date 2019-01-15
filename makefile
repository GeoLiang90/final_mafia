select: server client

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

mafia.o: mafia.c player.h
	gcc -c mafia.c

mafia: mafia.o player.o
	gcc -o mafia mafia.o player.o