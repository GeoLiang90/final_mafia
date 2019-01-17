#include "networking.h"

int send_name(int serv_sock){
  char name[20];
  printf("Please enter your nickname: ");
  fgets(name, 20, stdin);
  if (*name != '\n'){
    write(serv_sock,name,20);
    //printf("I ran \n");
    return 1;
  }
  return 0;
}
int main(int argc, char **argv) {
  int server_socket;
  char initial[BUFFER_SIZE];
  int sent_name = 0;
  int roster = 0;
  printf("[Client] Waiting for connection to server \n");
  server_socket = client_setup( TEST_IP );
  read(server_socket,initial,BUFFER_SIZE);
  //print out any initial messages from the server
  printf("%s \n",initial);
  while (1) {
    char state_read[BUFFER_SIZE];
    read(server_socket,state_read,BUFFER_SIZE);
    if (strcmp(state_read, "g0")== 0 && !(sent_name)){
      sent_name = send_name(server_socket);
    }
    if (strcmp(state_read, "g0")== 0 && sent_name){
      printf("Please Wait! Your name has been set \n");
    }
    if (strcmp(state_read, "g1")==0 && roster == 0){
      //Listen for a message
      char msg[BUFFER_SIZE];
      read(server_socket,msg,BUFFER_SIZE);
      printf("%s \n",msg);
      roster = 1;
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
