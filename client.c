#include "networking.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  server_socket = client_setup( TEST_IP );
  while (1) {
    read(server_socket,buffer,BUFFER_SIZE);
    printf("%s",buffer);
    if (strcmp(buffer,"Enter your nickname: ") == 0 ){
      char name[20];
      fgets(name, 20, stdin);
      write(server_socket,name,20);
    }

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
