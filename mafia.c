#include "mafia.h"
#include <string.h>
#include <unistd.h>
int PLAYER_COUNT = 3;
int T_WIN = 1;
int M_WIN = 2;
int S_WIN = 3;
int check_win(struct Player * p_list){
    int life_check = 0;
    for (int i = 0; i < PLAYER_COUNT; i++){
      if ((&p_list[i])-> isalive == 1){
        life_check += 1;
      }
    }
    if (life_check == (PLAYER_COUNT - 2)){
      int count_mafia = 0;
      int count_town = 0;
      for(int z = 0; z < PLAYER_COUNT; z++){
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "town") == 0)){
          count_town += 1;
        }
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "doctor") == 0)){
          count_town += 1;
        }
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "mafia") == 0)){
          count_mafia += 1;
        }
      }
      if (count_mafia == 0){
        return T_WIN;
      }
      if (count_mafia == 0 && count_town ==0){
        return S_WIN;
      }
      if (count_town == 0){
        return M_WIN;
      }
    }
    return 0;
  }

void assign_roles(int * socket_list, struct Player * p_list){
  int pop_count = 0;
  //While loop runs and assigns roles to corressponding players
  while (pop_count < PLAYER_COUNT){
    if (pop_count == 0){
      char buff[250];
      strcpy(buff,"You are assigned the role and alliance: Mafia \n");
      (&p_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "mafia");
      (&p_list[pop_count])-> perms = 1;
      write(socket_list[pop_count],buff,sizeof(buff));

    }
    else if (pop_count == 1){
      char buff[250];
      strcpy(buff,"You are assigned the role and alliance: Doctor  \n");
      (&p_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "doctor");
      (&p_list[pop_count])-> perms = 1;
      write(socket_list[pop_count],buff,sizeof(buff));
    }
    else {
      char buff[250];
      strcpy(buff,"You are assigned the role and alliance: Town Member \n");
      (&p_list[pop_count]) -> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "town");
      (&p_list[pop_count])-> perms = 0;
      write(socket_list[pop_count],buff,sizeof(buff));
    }
    pop_count += 1;
  }
  return;
}
void chop_space(char * string){
  int z = 0;
  while(string[z] != '\n'){
    z++;
  }
  string[z] = '\0';
  return;
}
void assign_name(int * socket_list, struct Player * p_list){
    int temp = fork();
    for(int i = 0; i < PLAYER_COUNT; i++){
      if (!temp){
        char name[25];
        //while(strlen(name) == 0){
          read(socket_list[i],name,25);
          chop_space(name);
        //}
        strcpy((&p_list[i])-> nickname,name);
      }
    }
        //exit(0);
    //}
  return;
}

int verify_names(struct Player * p_list){
  int count = 0;
  for(int z = 0; z < PLAYER_COUNT; z++){
    if (strlen((&p_list[z])->nickname) > 0) {
      count += 1;
    }
  }
  return count;
}
/* Don't use this method unless you can fix it
void write_client(int * socket_list, char * buf){
  //Assume the clients are already listening
  //int temp = fork();
  for (int i = 0; i < PLAYER_COUNT; i++){
    //int temp = fork();
    //if (!temp){
      write(socket_list[i],buf, sizeof(buf));
    //}
  }
  return;
}
*/
int run_game(int * socket_list){
  int game_state = 0;
  //Player Structs go here
  struct Player * player_list = (struct Player *) calloc(PLAYER_COUNT,sizeof(struct Player));
  assign_roles(socket_list,player_list);
  sleep(1);
  while(1){
  if(game_state == 0){
    for (int i = 0; i < PLAYER_COUNT; i++){
      //Basically tell the client to be ready to send stuff
        write(socket_list[i], "g0", 5);
    }
    assign_name(socket_list,player_list);
    //sleep(10);
    /*
    printf("%s \n",(&player_list[0]) -> nickname);
    printf("%s \n",(&player_list[1]) -> nickname);
    printf("%s \n",(&player_list[2]) -> nickname);
    */
    while(1){
      int verify = verify_names(player_list);
      //printf("%d \n", verify);
      if (verify == PLAYER_COUNT){
        printf("All users have set names \n");
        break;
      }
    }
    game_state  = 1;
  }// Game state ends hereeeee
    if (game_state == 1){
      printf("Pre Game: Sending Roster to all \n");
      char roster[256];
      strcpy(roster,"\nCurrent Roster: \n");
      for (int i = 0; i < PLAYER_COUNT; i++){
	strcat(roster,"\t-");
        strcat(roster,((&player_list[i])-> nickname));
        strcat(roster,"\n");
      }
      strcat(roster,"\n");
      printf("%s \n", roster);
      //These do not work properly until you fix write_client
      /*
      write_client(socket_list,"g1");
      write_client(socket_list,roster);
      */

      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g1",5);
        write(socket_list[i],roster,256);
      }
      game_state += 1;
    }
    if(game_state == 2){
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g2",5);
      }
      break;
      //Course through the list of player and check isDead then list who is connected
      if (check_win(player_list) != 0){
        game_state += 2;
      }
    }
    if(game_state == 3){
      printf("Night Time: Perform your actions if you have one");
      //So I think structs will have the will stored there but also the actions that each person can do
    }
    if(game_state == 4){
      printf("Game End");
      //break;
    }
  }
  return 0;
}
