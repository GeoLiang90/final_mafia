#include "networking.h"

void process(char *s);
void subserver(int *from_clients);

int main() {

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  listen_socket = server_setup();

  while (1) {
    printf("[server] new game\n");
    printf("[server] waiting for players\n");

    int player_count;
    int * player_list;

    player_count = 0;
    player_list = (int *) calloc(6, sizeof(int));
    while (player_count < 6) {
      player_list[player_count] = server_connect(listen_socket);
      printf("[server] player%d has connected\n", player_count + 1);
      player_count++;
    }
    
    /*
    int client_socket = server_connect(listen_socket);
    printf("[server] player1 has connected\n");
    */
    //wait for multiple clients to connect
    //create list of client sockets,
    
    f = fork();
    if (!f) {
      printf("[subserver %d] starting game\n", getpid());
      exit(0);
    }
    close(client_socket);
  }
}

void subserver(int * client_sockets) {
  char buffer[BUFFER_SIZE];
  /*
  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
  */
}
