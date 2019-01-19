#include "networking.h"
#include "mafia.h"
void process(char *s);
void subserver(int *from_clients);

static void sighandler(int n){
  if(n == SIGINT || n == SIGKILL){
    printf("[server] exiting");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGKILL, sighandler);

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  listen_socket = server_setup();

  //while (1) {
    printf("[server] new game\n");
    printf("[server] waiting for players\n");

    int p_count;
    int * player_list;

    p_count = 0;
    player_list = (int *) calloc(PLAYER_COUNT, sizeof(int));
    while (p_count < PLAYER_COUNT) {
      //wait for multiple clients to connect
      //create list of client sockets,
      player_list[p_count] = server_connect(listen_socket);
      printf("[server] Player %d has connected\n", p_count + 1);
      p_count++;
    }
    //f = fork();
    //if (!f) {
      //printf("[subserver %d] starting game\n", getpid());
      run_game(player_list);
      //exit(0);
    //}
    while (!(p_count == 0)){
      close(player_list[p_count-1]);
      p_count--;
    }
  //}
}
