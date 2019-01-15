#include <time.h>
#include "player.h"
#include <stdio.h>

int run_game(int * socket_list){
  int game_state = 0;
  //Player Structs go here
  struct Player * player_list = (struct Player *) calloc(6,sizeof(struct Player));
  int pop_count = 0

  while (pop_count < 6){

    if (pop_count == 0){
      player_list[0]-> socket_fd = socket_list[0];
    }
    if (pop_count == 1){
      player_list[1]-> socket_fd = socket_list[1];
    }
    else {
      player_list[pop_count] -> socket_fd = socket_list[pop_count];
    }
    player_count += 1;
  }

  while(1){
    if (game_state == 0){
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
      break;
  }
  }
  return 0;
}
