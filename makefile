select: server client

server: server.o networking.o mafia.o
	gcc -o server server.o networking.o mafia.o

client: client.o networking.o
	gcc -o client client.o networking.o

server.o: server.c networking.h mafia.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

mafia.o: mafia.c player.h
	gcc -c mafia.c

clean:
	rm *.o
