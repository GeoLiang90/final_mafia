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
      strcpy(buff,"You are assigned the role and alliance: Serial Killer  \n");
      (&p_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "sk");
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
      //printf("I get here \n");
      write(socket_list[pop_count],buff,sizeof(buff));
    }
    pop_count += 1;
  }
  return;
}
int run_game(int * socket_list){
  int game_state = 0;
  //Player Structs go here
  struct Player * player_list = (struct Player *) calloc(PLAYER_COUNT,sizeof(struct Player));
  assign_roles(socket_list,player_list);
  if(game_state == 0){
    char name[20];
    //printf("%d \n", PLAYER_COUNT);
    for (int i = 0; i < PLAYER_COUNT; i++){
      write(socket_list[i], "g0", 20);
      int temp = fork();
      if (temp){
        read(socket_list[i],name,20);
        strcpy((&player_list[i])-> nickname,name);
      }
    }
    char names[PLAYER_COUNT][21];
    char name_list[(PLAYER_COUNT + 1) * 30];
    strcpy(name_list,"Current Roster: \n");
    for (int i = 0; i < PLAYER_COUNT; i++){
      strcpy(names[i],(&player_list[i])-> nickname);
      strcat(name_list,((&player_list[i])-> nickname));
      strcat(name_list,"\n");
    }
    while(1){
      int entered = 0;
      printf("%d", entered);
      if (entered == PLAYER_COUNT){
        for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i], "Game is about to start \n", 40);
        write(socket_list[i], name_list,sizeof(name_list));
        }
        break;
      }
      if (name_list[entered]){
        entered += 1;
      }
    }
    //game_state += 1;
    }
      //Pre-Game goes here
      //printf("Pre-Game: Enter your alias");
      //make sure the clients get appropriate structs and what not
    if(game_state == 1){
      printf("Day Time: The sun has now risen");
      //Course through the list of player and check isDead then list who is connected
      if (check_win(player_list) != 0){
        game_state += 2;
      }
    }
    if(game_state == 2){
      printf("Night Time: Perform your actions if you have one");
      //So I think structs will have the will stored there but also the actions that each person can do
    }
    if(game_state == 3){
      printf("Game End");
    }
  return 0;
}
