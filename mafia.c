#include "mafia.h"

int check_win(struct Player * p_list){
    int life_check = 0;
    for (int i = 0; i < PLAYER_COUNT; i++){
      if ((&p_list[i])-> isalive == 1){
        life_check += 1;
      }
    }
    if (life_check == (PLAYER_COUNT - 2)){
      int count_mafia = 0;
      int count_sk = 0;
      int count_town = 0;
      for(int z = 0; z < PLAYER_COUNT; z++){
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role,"town") == 0)){
          count_town += 1;
        }
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role,"sk") == 0)){
          count_mafia += 1;
        }
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role,"mafia") == 0)){
          count_sk += 1;
        }
      }
      if (count_mafia == 0 && count_sk == 0){
        return T_WIN;
      }
      if (count_mafia == 0 && count_town ==0){
        return S_WIN;
      }
      if (count_sk == 0 && count_town == 0){
        return M_WIN;
      }
    }
    return 0;
}
int run_game(int * socket_list){
  int game_state = 0;
  //Player Structs go here
  struct Player * player_list = (struct Player *) calloc(PLAYER_COUNT,sizeof(struct Player));
  int pop_count = 0;
  //While loop runs and assigns roles to corressponding players
  while (pop_count < PLAYER_COUNT){
    if (pop_count == 0){
      (&player_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&player_list[pop_count])-> isalive = 1;
      strcpy((&player_list[pop_count])-> role, "mafia");
      (&player_list[pop_count])-> perms = 1;
    }
    if (pop_count == 1){
      (&player_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&player_list[pop_count])-> isalive = 1;
      strcpy((&player_list[pop_count])-> role, "mafia");
      (&player_list[pop_count])-> perms = 1;
    }
    else {
      (&player_list[pop_count]) -> socket_fd = socket_list[pop_count];
      (&player_list[pop_count])-> isalive = 1;
      strcpy((&player_list[pop_count])-> role, "mafia");
      (&player_list[pop_count])-> perms = 0;
    }
    pop_count += 1;
  }

  if (game_state == 0){
    char name[20];
    for (int i = 0; i < PLAYER_COUNT; i++){
      printf("Test");
      write(socket_list[i], "Enter your nickname: ", 20);
      read(socket_list[i],name,20);
      strcpy((&player_list[i])-> nickname,name);
    }
      //Pre-Game goes here
      //printf("Pre-Game: Enter your alias");
      //make sure the clients get appropriate structs and what not
    }
    if (game_state == 1){
      printf("Day Time: The sun has now risen");
      //Course through the list of player and check isDead then list who is connected
    }
    if (game_state == 2){
      printf("Night Time: Perform your actions if you have one");
      //So I think structs will have the will stored there but also the actions that each person can do
    }
    if (game_state == 3){
      printf("Game End");
  }
  return 0;
}
