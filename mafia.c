#include "mafia.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
int PLAYER_COUNT = 3;
int T_WIN = 1;
int M_WIN = 2;
int S_WIN = 3;


int game_state = 0;

void assign_roles(int * socket_list, struct Player * p_list){
  int pop_count = 0;
  //While loop runs and assigns roles to corressponding players
  while (pop_count < PLAYER_COUNT){
    if (pop_count == 0){
      char b[250];
      strncpy(b,"You are assigned the role and alliance: Mafia \n",sizeof("You are assigned the role and alliance: Mafia \n"));
      (&p_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strncpy((&p_list[pop_count])-> role, "mafia", 5);
      (&p_list[pop_count])-> perms = 1;
      (&p_list[pop_count])-> votes = 0;
      (&p_list[pop_count])-> announced = 0;
      (&p_list[pop_count])-> executed = 0;
      (&p_list[pop_count])-> exec_a = 0;
      write(socket_list[pop_count],b,sizeof(b));
      //sleep(1);

    }
    else {
      char buff[250];
      strncpy(buff,"You are assigned the role and alliance: Town Member \n",sizeof("You are assigned the role and alliance: Town Member \n"));
      (&p_list[pop_count]) -> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strncpy((&p_list[pop_count])-> role, "town", 4);
      (&p_list[pop_count])-> perms = 0;
      (&p_list[pop_count])-> votes = 0;
      (&p_list[pop_count])-> announced = 0;
      (&p_list[pop_count])-> executed = 0;
      (&p_list[pop_count])-> exec_a = 0;
      write(socket_list[pop_count],buff,sizeof(buff));
      //sleep(1);
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
        strncpy((&p_list[i])-> nickname,name,sizeof(name));
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

int verify_action(struct Player * p_list){
  int count = 0;
  for(int z = 0; z < PLAYER_COUNT; z++){
    if ((&p_list[z])-> isalive){
      if (strlen((&p_list[z])->action) > 0) {
        count += 1;
      }
    }
    else {
      count += 1;
    }
  }
  return count;
}

int verify_vote(struct Player * p_list){
  int count = 0;
  for (int z = 0; z < PLAYER_COUNT; z++){
    if ((&p_list[z])-> isalive){
      if((&p_list[z])->votes){
        count += 1;
      }
    }
    else{
      count += 1;
    }
  }
  return count;
}

void action(int * socket_list, struct Player * p_list){
  int t = fork();
  char killed[20];
  for(int i = 0; i < PLAYER_COUNT; i++){
   if (!t && (&p_list[i])-> isalive){
      char temp[20];

        read(socket_list[i],temp,20);

        chop_space(temp);
      //}
      strncpy((&p_list[i])-> action,temp,sizeof(temp));
      if (strncmp(temp,"written",sizeof("written")) != 0){
        strncpy(killed,(&p_list[i])-> action,sizeof((&p_list[i])-> action));
      }
    }
  }
  for(int i = 0; i < PLAYER_COUNT; i++){
    if (strncmp((&p_list[i])-> nickname,killed,sizeof(killed)) == 0){
      //printf("I ran \n");
      (&p_list[i])-> isalive = 0;
    }
  }
      //exit(0);
  //}
  return;
  }


int daytime_pre(int * socket_list, struct Player * p_list){
  char killed[20];
  char killmsg[250];
  int die_count = 0;
  int id = -1;

  for (int i = 0; i < PLAYER_COUNT; i++){
    if ((&p_list[i]) -> isalive == 0 && (&p_list[i]) -> announced == 0){
      strncpy(killed,(&p_list[i]) -> nickname, sizeof((&p_list[i]) -> nickname));
      (&p_list[i]) -> announced = 1;
      strncpy(killmsg,"Last night, a member died: ",sizeof("Last night, a member died: "));
      strncat(killmsg,killed,sizeof(killed));
      id = i;
    }
  }
  if (strlen(killmsg) == 0){
    strncpy(killmsg,"Last night, nobody died.",sizeof("Last night, nobody died."));
  }

  for (int i = 0; i < PLAYER_COUNT; i++){
    write(socket_list[i],killmsg,sizeof(killmsg));
    sleep(1);
  }
  return id;
}

void handle_vote(int * socket_list, struct Player * p_list){
  for (int z = 0; z < PLAYER_COUNT; z++){
    (&p_list[z]) -> votes = 0;
  }

  int * votes = calloc(PLAYER_COUNT,sizeof(int));

  for (int i = 0; i < PLAYER_COUNT; i++){
    if ((&p_list[i])-> isalive) {
      char n[25];

      (&p_list[i]) -> votes = 1;
      read(socket_list[i],n,25);

      sleep(1);

      for (int x = 0; x < PLAYER_COUNT; x++){
        if (strcmp(n,(&p_list[x]) -> nickname) == 0){
          votes[x] += 1;
        }
      }
    }
  }
  int position = -1;
  int highest = 0;
  for (int z = 0; z < PLAYER_COUNT; z++){
    if (votes[z] > highest){
      position = z;
      highest = votes[z];
    }
  }
  for (int a = 0; a < PLAYER_COUNT; a++){
    printf("%s ",(&p_list[a]) -> nickname);
    printf("%d \n",votes[a]);
  }
  printf("%s", (&p_list[position]) -> nickname);
  printf("%d \n",highest);
  (&p_list[position]) -> isalive = 0;
  (&p_list[position]) -> announced = 1;
  (&p_list[position]) -> executed = 1;
  /*
  char end[50];
  strcpy(end,"Farewell cruel world this person has passed: ");
  strcat(end,(&p_list[position]) -> nickname);
  for (int v = 0; v < PLAYER_COUNT; v++){
    write(socket_list[v], end, 50);
  }
  */
  return;
}

void announce_executed(int * socket_list,struct Player* player_list){
  for (int i = 0; i < PLAYER_COUNT; i++){
    if((&player_list[i]) -> executed && !((&player_list[i]) -> exec_a) ){
      char end[50];
      strcpy(end,"Farewell cruel world this person has passed: ");
      (&player_list[i]) -> exec_a = 1;
      strcat(end,(&player_list[i]) -> nickname);
      for (int v = 0; v < PLAYER_COUNT; v++){
        write(socket_list[v], end, 50);
        sleep(1);
      }
    }
  }
}

int check_win(struct Player * p_list){
    int life_check = 0;
    for (int i = 0; i < PLAYER_COUNT; i++){
      if ((&p_list[i])-> isalive == 1){
        life_check += 1;
      }
    }
    if (life_check < PLAYER_COUNT){
      int count_mafia = 0;
      int count_town = 0;
      for(int z = 0; z < PLAYER_COUNT; z++){
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role,"town") == 0)){
          count_town += 1;
        }
        if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role,"mafia") == 0)){
          count_mafia += 1;
        }
      }
      if (count_mafia == 1 && count_town ==0){
        return M_WIN;
      }
      if (count_mafia == 0 && count_town >0){
        return T_WIN;
      }
    }
    return 0;
}


int run_game(int * socket_list){
  //Player Structs go here
  struct Player * player_list = (struct Player *) calloc(PLAYER_COUNT,sizeof(struct Player));
  assign_roles(socket_list,player_list);
  sleep(1);
  while(1){
  if(game_state == 0){
    for (int i = 0; i < PLAYER_COUNT; i++){
      //Basically tell the client to be ready to send stuff
        write(socket_list[i], "g0", 5);
        sleep(1);
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
      strncpy(roster,"\nCurrent Roster: \n", sizeof("\nCurrent Roster: \n"));
      for (int i = 0; i < PLAYER_COUNT; i++){
	       strncat(roster,"\t-",sizeof("\t-"));
        strncat(roster,((&player_list[i])-> nickname),sizeof((&player_list[i])-> nickname));
        strncat(roster,"\n",sizeof("\n"));
      }
      strncat(roster,"\n",sizeof("\n"));
      printf("%s \n", roster);

      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g1",5);

        //sleep(1);
        write(socket_list[i],roster,sizeof(roster));
        //sleep(1);
      }
      //checkWin(player_list);
      game_state += 1;
    }
    if(game_state == 2){

      for (int i = 0; i < PLAYER_COUNT; i++){
        if ((&player_list[i])-> isalive){
        write(socket_list[i],"g2",5);
      }
        //sleep(1);
      }
      action(socket_list,player_list);
      sleep(1);
      while(1){
        if(verify_action(player_list) == PLAYER_COUNT){
          break;
        }
      }
      game_state += 1;
      // }
    }
    if(game_state == 3){
      for (int i = 0; i < PLAYER_COUNT; i++){
        if ((&player_list[i])-> isalive){
        write(socket_list[i],"g3",5);
      }
        //sleep(1);
      }
      daytime_pre(socket_list,player_list);
      game_state += 1;
    }
    if (game_state == 4){
      for (int i = 0; i < PLAYER_COUNT; i++){
        if ((&player_list[i])-> isalive){
        write(socket_list[i],"g4",5);
      }
        //sleep(1);
      }
      handle_vote(socket_list,player_list);
      sleep(1);
      while(1){
        if(verify_vote(player_list) == PLAYER_COUNT){
          break;
        }
      }
      printf("I progressed to the next state \n");
      game_state +=1;
    }
    if (game_state == 5){
      for (int i = 0; i < PLAYER_COUNT; i++){
        if ((&player_list[i])-> isalive){
        write(socket_list[i],"g5",5);
      }
        //sleep(1);
      }
      printf("I progressed to 5 state \n");
      announce_executed(socket_list,player_list);
      //sleep(2);
      printf("CHANGING GAME STATE \n");
      printf("%d \n", game_state);
      game_state -= 3;
      printf("%d \n", game_state);
    }
  }
}
