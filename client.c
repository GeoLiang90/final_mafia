#include "networking.h"
#include "mafia.h"
int send_name(int serv_sock){
  char name[20];
  printf("Please enter your nickname: ");
  fgets(name, 20, stdin);
  if (*name != '\n'){
    write(serv_sock,name,20);
    return 1;
  }
  return 0;
}
int main(int argc, char **argv) {
  int server_socket;

  int sent_name = 0;
  int roster = 0;
  printf("[Client] Waiting for connection to server \n");
  server_socket = client_setup(TEST_IP);

  while (1) {
    char state_read[5];
    read(server_socket,state_read,5);
    if (strcmp(state_read, "g0")== 0 && !(sent_name)){
      sent_name = send_name(server_socket);
    }
    if (strcmp(state_read, "g0")== 0 && sent_name == 1){
      printf("Please Wait! Your name has been set \n");
      sent_name += 1;
    }

    if (strcmp(state_read, "g1")==0 && roster == 0){
      //Listen for a message
      char msg[BUFFER_SIZE];
      read(server_socket,msg,256);
      printf("%s \n",msg);
      roster += 1;
    }

  }
}
