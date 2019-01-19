#include "networking.h"
#include "mafia.h"
#include <time.h>
#include <string.h>
char role[20];
char name[20];
char will_statement[250];
int server_socket;
char buff[BUFFER_SIZE];
int sent_name = 0;
int roster = 0;
int day = 0;

int dead = 0;

int performed = 0;

void chop_space(char * string){
  int z = 0;
  while(string[z] != '\n'){
    z++;
  }
  string[z] = '\0';
  return;
}

int send_name(int serv_sock){
  printf("Please enter your nickname: ");
  fgets(name, 20, stdin);
  write(serv_sock,name,20);
  return 1;
}
void do_action(int serv_sock){
  char rec[3];
  if (!dead) {
    printf("Here are your actions: \n");
    if (strncmp(role,"Mafia",sizeof("Mafia")) == 0){
      printf("[1]: Write Will \n");
      printf("[2]: Kill \n");
    }
    else if (strncmp(role,"Town Member",sizeof("Town Member")) == 0){
      printf("[1]: Write Will \n");
    }
    printf("Enter a number corresponding to your actions: ");
    fgets(rec,3,stdin);
    chop_space(rec);
    if (strncmp(rec,"1",sizeof(1)) == 0) {
      printf("Write your will: ");
      fgets(will_statement,250,stdin);
      //printf("%s \n",will_statement);
      write(serv_sock,"written",sizeof("written"));
    }
    if(strncmp(role,"Mafia",sizeof("Mafia")) ==0 && strncmp(rec,"2",sizeof("2")) == 0 ){
      printf("Enter the name of the player you want to kill: ");
      char victim[20];
      fgets(victim,20,stdin);
      //printf("%s \n",victim);
      write(serv_sock,victim,sizeof(victim));
    }
  }
  return;
}

void do_vote(int serv_sock){
  if (!dead) {
    char n[25];
    printf("Enter the player you want to eliminate. Everyone must vote!: ");
    fgets(n,25,stdin);
    chop_space(n);
    write(serv_sock, n, sizeof(n));
    sleep(1);
  }
}

void send_msg(int serv_sock, char * msg){
  write(serv_sock,msg,sizeof(msg));
}



int main(int argc, char **argv) {
  printf("[Client] Waiting for connection to server \n");
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );
  read(server_socket,buff,BUFFER_SIZE);
  //sleep(1);
  if (strstr(buff,"Mafia")){
    strncpy(role,"Mafia",sizeof("Mafia"));
  }
  else{
    strncpy(role, "Town Member",sizeof("Town Member"));
  }
  //print the initial message
  printf("%s", buff);

  while (1) {
    char state_read[5];
    read(server_socket,state_read,5);
    //sleep(1);
    if (strncmp(state_read, "g0",sizeof("g0"))== 0 && !(sent_name)){
      //printf("Worked");
      sent_name = send_name(server_socket);
    }
    if (strncmp(state_read, "g0",sizeof("g0"))== 0 && sent_name == 1){
      printf("Please Wait! Your name has been set \n");
      sent_name += 1;
    }
    if (strncmp(state_read, "g1",sizeof("g1"))==0 && roster == 0){
      //Listen for a message
      printf("Role: %s \n",role);
      printf("Game is starting soon... \n");
      char msg[BUFFER_SIZE];
      read(server_socket,msg,BUFFER_SIZE);
      sleep(1);
      printf("%s",msg);
      roster += 1;
    }

    if (strncmp(state_read,"g2",sizeof("g2"))==0 && !(performed)){
      day += 1;
      printf("NIGHT %d \n",day);
      do_action(server_socket);
      performed += 1;
    }

    if(strncmp(state_read,"g2",sizeof("g2")) == 0 && performed == 1){
      char msg[50];
      strncpy(msg,"The night ends soon...\n",sizeof("The night ends soon... \n"));
      //sleep(1);
      printf("%s",msg);
      sleep(1);
      performed += 1;
      //sleep(1);
    }

    if(strncmp(state_read,"g3",sizeof("g3")) == 0){
      //printf("stage 3");
      performed = 0;
      char m[BUFFER_SIZE];
      //sleep(1);
      read(server_socket,m,BUFFER_SIZE);
      if (strstr(m,name) != NULL){
        printf("dead\n");
        dead = 1;
      }
      printf("%s \n",m);
      sleep(1);
    }
    if(strncmp(state_read,"g4",sizeof("g4")) == 0){
      printf("DAY %d \n",day);
      do_vote(server_socket);
    }
    if(strncmp(state_read,"g5",sizeof("g5")) == 0){

      char e[BUFFER_SIZE];
      read(server_socket,e,BUFFER_SIZE);
      if (strstr(e, name) != NULL){
        printf("dead\n");
        dead = 1;
      }
      printf("%s \n",e);
      printf("Dawn is falling and the night shall soon arise! \n");
      strncpy(state_read,"g2",sizeof("g2"));
      //sleep(1);
    }
    if(strncmp(state_read,"dead",sizeof("dead")) == 0){
      dead = 1;
      printf("You have died! \n");
    }
    if(strncmp(state_read,"tw",sizeof("tw")) == 0){
      printf("The town wins!\n");
      return 0;
    }
    if(strncmp(state_read,"mw",sizeof("mw")) == 0){
      printf("The mafia wins!\n");
      return 0;
    }
  }
  return 0;
}
