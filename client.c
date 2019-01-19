#include "networking.h"
#include "mafia.h"

char role[20];
char name[20];
int server_socket;
char buff[BUFFER_SIZE];
int sent_name = 0;
int roster = 0;
int day = 0;
int dead = 0;

int send_name(int serv_sock){
  printf("Please enter your nickname: ");
  fgets(name, 20, stdin);
  write(serv_sock,name,20);
  return 1;
}

void send_msg(int serv_sock, char * msg){
  write(serv_sock,msg,sizeof(msg));
}

int main(int argc, char **argv) {
  printf("[Client] Waiting for connection to server \n");
  server_socket = client_setup(TEST_IP);
  read(server_socket,buff,BUFFER_SIZE);

  if (strstr(buff,"Mafia")){
    strcpy(role,"Mafia");
  }
  else if(strstr(buff,"Doctor")){
    strcpy(role,"Doctor");
  }
  else{
    strcpy(role, "Town Member");
  }
  //print the initial message
  printf("%s", buff);

  while (1) {
    if (dead == 1) {
      return;
    }
    char state_read[5];
    read(server_socket,state_read,5);
    if (strcmp(state_read, "g0")== 0 && !(sent_name)){
      //printf("Worked");
      sent_name = send_name(server_socket);
    }
    if (strcmp(state_read, "g0")== 0 && sent_name == 1){
      printf("Please Wait! Your name has been set \n");
      sent_name += 1;
    }

    if (strcmp(state_read, "g1")==0 && roster == 0){
      //Listen for a message
      printf("Role: %s \n",role);
      printf("Game is starting soon... \n");
      char msg[BUFFER_SIZE];
      read(server_socket,msg,BUFFER_SIZE);
      printf("%s",msg);
      roster += 1;
    }
    if (strcmp(state_read,"g2")==0){
      nightTime();
    }
    if (strcmp(state_read,"g3")==0){
      // printf("Does this run??\n");
      char msg[BUFFER_SIZE];
      read(server_socket,msg,BUFFER_SIZE);
      printf("%s\n",msg);
      char *pch = strstr(msg, "you have died");
      if (pch) {
          printf("You have lost...\n");
          dead = 1;
      }
      writeWill();
    }
    if (strcmp(state_read,"g4")==0) {
      printf("Who would you like to suspect? (Vote for nobody by hitting enter): ");
      getData(server_socket);
    }
    if (strcmp(state_read,"g5")==0) {
      char msg[BUFFER_SIZE];
      read(server_socket,msg,BUFFER_SIZE);
      printf("%s\n",msg);
      char *pch = strstr(msg, "you have died");
      if (pch) {
          printf("You have lost...\n");
          dead = 1;
      }
    }
    if (strcmp(state_read,"g6")==0) {
      printf("Game Over! Mafia win!\n");
    }
    if (strcmp(state_read,"g7")==0) {
      printf("Game Over! Mafia lose!\n");
    }
  }
}

int getData(int serv_sock){
  fgets(name, 20, stdin);
  write(serv_sock,name,20);
  return 1;
}

void writeWill () {
  char will[100];
  day += 1;
  printf("---------------------------\n");
  printf("Days Past: %d \n",day);
  printf("(MORNING) \n");
  printf("---------------------------\n");
  if (dead == 0) {
    printf("Please write your will: ");
    fgets(will, 100, stdin);
    write(server_socket, will, 100);
  } else {
    write(server_socket, "", 100);
  }
}

void nightTime () {
      day += 1;
      printf("---------------------------\n");
      printf("Days Past: %d \n",day);
      printf("(NIGHT) \n");
      printf("---------------------------\n");
      if (strcmp(role, "Doctor") == 0) {
	       printf("Which person would you like to heal?: ");
         getData(server_socket);
      }
      if (strcmp(role, "Mafia") == 0) {
	       printf("Which person would you like to kill?: ");
         getData(server_socket);
      }
      if (strcmp(role, "Town Member") == 0) {
	       printf("<Town members cannot do anything during the night> \n");
         write(server_socket,"N/A",20);
      }
}
