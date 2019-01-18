#include "networking.h"
#include "mafia.h"
//
char role[20];
char name[20];
int server_socket;
char buff[BUFFER_SIZE];
int sent_name = 0;
int roster = 0;
int day = 0;

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
      dayTime();
    }
    if (strcmp(state_read,"g2")==0){
      //Depends on the players role.
    }

  }
}

int getData(int serv_sock){
  fgets(name, 20, stdin);
  write(serv_sock,name,20);
  return 1;
}

void dayTime () {
      day += 1;
      printf("The sun has risen and it's time to wake up\n");
      printf("---------------------------\n");
      printf("Days Past: %d \n",day);
      printf("---------------------------\n");
      if (strcmp(role, "Doctor") == 0) {
	       printf("Which person would you like to heal?: ");
         getData();
      }
      if (strcmp(role, "Mafia") == 0) {
	       printf("Which person would you like to kill?: ");
         getData();
      }
      if (strcmp(role, "Town Member") == 0) {
	       printf("<Town Members cannot perform any actions during night>\n");
      }
}