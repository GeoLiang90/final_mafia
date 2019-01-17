#include "networking.h"

void send_name(int serv_sock){
  char name[20];
  printf("Please enter your nickname: ");
  fgets(name, 20, stdin);
  write(serv_sock,name,20);
}
int main(int argc, char **argv) {
  int server_socket;
  char initial[BUFFER_SIZE];
  printf("[Client] Waiting for connection to server \n");
  server_socket = client_setup( TEST_IP );
  read(server_socket,initial,BUFFER_SIZE);
  //print out any initial messages from the server
  printf("%s \n",initial);
  while (1) {
    char state_read[BUFFER_SIZE];
    read(server_socket,state_read,BUFFER_SIZE);
    if (strcmp(state_read, "g0")==0){
      char msg[BUFFER_SIZE];
      send_name(server_socket);
      read(server_socket,msg,BUFFER_SIZE);
      printf("%s",msg);
    }
    /*
    if (strcmp(buffer,"Enter your nickname: ") == 0 ){
      char name[20];
      fgets(name, 20, stdin);
      write(server_socket,name,20);
    }
    */
    //printf("testing");
    /*
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
    */
  }
}
