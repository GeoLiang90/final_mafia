#include "networking.h"
#include "mafia.h"
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
      //wait for multiple clients to connect
      //create list of client sockets,
      player_list[player_count] = server_connect(listen_socket);
      printf("[server] Player %d has connected\n", player_count + 1);
      player_count++;
    }
    f = fork();
    if (!f) {
      printf("[subserver %d] starting game\n", getpid());
      run_game(player_list);
      exit(0);
    }
    while (!(player_count == 0)){
      close(player_list[player_count-1]);
      player_count--;
    }
  }
}
