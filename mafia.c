#include "mafia.h"
#include <string.h>
#include <unistd.h>
int PLAYER_COUNT = 3;
int T_WIN = 1;
int M_WIN = 2;
int S_WIN = 3;

// game state temporarily outside of run_game
// wills cant be empty
// sometimes messages get sent two times

// int check_win(struct Player * p_list){
//     int life_check = 0;
//     for (int i = 0; i < PLAYER_COUNT; i++){
//       if ((&p_list[i])-> isalive == 1){
//         life_check += 1;
//       }
//     }
//     if (life_check == (PLAYER_COUNT - 2)){
//       int count_mafia = 0;
//       int count_town = 0;
//       for(int z = 0; z < PLAYER_COUNT; z++){
//         if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "town") == 0)){
//           count_town += 1;
//         }
//         if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "doctor") == 0)){
//           count_town += 1;
//         }
//         if (((&p_list[z]) -> isalive == 1) && (strcmp((&p_list[z]) -> role, "mafia") == 0)){
//           count_mafia += 1;
//         }
//       }
//       if (count_mafia == 0){
//         return T_WIN;
//       }
//       if (count_mafia == 0 && count_town ==0){
//         return S_WIN;
//       }
//       if (count_town == 0){
//         return M_WIN;
//       }
//     }
//     return 0;
//   }

int checkWin(struct Player * p_list) {
  int nonMafiaCount = 0;
  int mafiaCount = 0;
  for (int i = 0; i < PLAYER_COUNT; i++) {
    if (strcmp((&p_list[i])->role, "doctor") == 0) {
      if ((&p_list[i])->isalive == 1) {
        nonMafiaCount++;
      }
    }
    if (strcmp((&p_list[i])->role, "town") == 0) {
      if ((&p_list[i])->isalive == 1) {
        nonMafiaCount++;
      }
    }
    if (strcmp((&p_list[i])->role, "mafia") == 0) {
      if ((&p_list[i])->isalive == 1) {
        mafiaCount++;
      }
    }
  }
  if (mafiaCount == 0) {
      // mafia lose
    game_state = 6;
    return 0;
  } if (nonMafiaCount <= 1) {
     // mafia win
    game_state = 7;
    return 1;
  } else {
    // nothing
    return -1;
  }
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
      (&p_list[pop_count])-> votes = 0;
      write(socket_list[pop_count],buff,sizeof(buff));

    }
    else if (pop_count == 1){
      char buff[250];
      strcpy(buff,"You are assigned the role and alliance: Doctor  \n");
      (&p_list[pop_count])-> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "doctor");
      (&p_list[pop_count])-> perms = 1;
      (&p_list[pop_count])-> votes = 0;
      write(socket_list[pop_count],buff,sizeof(buff));
    }
    else {
      char buff[250];
      strcpy(buff,"You are assigned the role and alliance: Town Member \n");
      (&p_list[pop_count]) -> socket_fd = socket_list[pop_count];
      (&p_list[pop_count])-> isalive = 1;
      strcpy((&p_list[pop_count])-> role, "town");
      (&p_list[pop_count])-> perms = 0;
      (&p_list[pop_count])-> votes = 0;
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
int game_state = 0;
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
      checkWin(player_list);
      game_state += 1;
    }
    if(game_state == 2){
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g2",5);
      }
      getAction(socket_list, player_list);
      // printf("Hello\n");
      while(1){
        // printf("in this loop\n");
        int verify = verify_action(player_list);
        // printf("amount of people who have given in action: %d\n", verify);
        //printf("%d \n", verify);
        if (verify == PLAYER_COUNT){
          printf("All users have done their action \n");
          break;
        }
      }
      checkWin(player_list);
      game_state += 1;

      // if (check_win(player_list) != 0){
      //   game_state += 3;
      // }
    }
    if(game_state == 3){
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g3",5);
      }
      doActions(socket_list,player_list);
      getWills(socket_list,player_list);
      while (1) {
        int verify = verify_wills(player_list);
        // printf("amount of people who have given in action: %d\n", verify);
        //printf("%d \n", verify);
        if (verify == PLAYER_COUNT){
          printf("All users have done submitted their wills \n");
          break;
        }
      }
      checkWin(player_list);
      game_state = 4;
    }
    if (game_state == 4) {
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g4",5);
      }
      getAction(socket_list, player_list);

      while(1){
        // printf("in this loop\n");
        int verify = verify_action(player_list);
        // printf("amount of people who have given in action: %d\n", verify);
        //printf("%d \n", verify);
        if (verify == PLAYER_COUNT){
          printf("All users have done inputted their suspects \n");
          break;
        }
      }
      checkWin(player_list);
      game_state = 5;
    }
    if (game_state == 5) {
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g5",5);
      }
      suspectKill(socket_list,player_list);
      sleep(2);
      checkWin(player_list);
      game_state = 2;
    }
    if(game_state == 6){
      // Mafia lose
      printf("Game End! Mafia lost!");
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g6",5);
      }
      //break;
    } if (game_state == 7) {
      // Mafia win
      printf("Game End! Mafia won!");
      for (int i = 0; i < PLAYER_COUNT; i++){
        write(socket_list[i],"g7",5);
      }
    }
  }
  return 0;
}

/* PLAYER WITH MOST VOTES
// for each player
for(int i = 0; i < PLAYER_COUNT; i++){
  // find nickname
  for (int z = 0; i < PLAYER_COUNT; i++) {
    if (strcmp((&p_list[i])-> action, (&p_list[z])-> nickname) == 0){
      // add a vote
      (&p_list[z])-> votes += 1;
    }
  }
}

char playerToKill[20];
int votes = 0;

for (int i = 0; i < PLAYER_COUNT; i++){
  if ((&p_list[i])->votes > votes){
    strcpy(playerToKill, (&p_list[i])->nickname);
    votes = (&p_list[i])->votes;
  }
}
*/

void suspectKill (int * socket_list, struct Player * p_list) {
    int number = rand() % (PLAYER_COUNT);
    char playerToKill[20];
    char playerToDieWill[100];

    strcpy(playerToKill, (&p_list[number])->action);
    printf("Player to Kill: %s\n", playerToKill);
    if (strlen(playerToKill) > 0) {
      for (int i = 0; i < PLAYER_COUNT; i++) {
        if (strcmp(playerToKill, (&p_list[i])-> nickname) == 0){
          strcpy(playerToDieWill, (&p_list[i])-> will_statement);
        }
      }

      for (int i = 0; i < PLAYER_COUNT; i++) {
        if (strcmp(playerToKill, (&p_list[i])-> nickname) == 0) {
          char buff[250];
          strcpy(buff,"A random suspect was chosen and it was you -- you have died! \n");
          (&p_list[i])-> isalive = 0;
          write(socket_list[i],buff,sizeof(buff));
        }
        else {
          char buff[250];
          strcat(buff, "One of the suspected people was killed, his name was ");
          strcat(buff, playerToKill);
          if (strcmp(playerToDieWill, "") == 0) {
            strcat(buff, ". He did not have a will yet." );
          } else {
            strcat(buff,". His will was: '");
            strcat(buff, playerToDieWill);
            strcat(buff, "'.\n");
          }
          write(socket_list[i],buff,sizeof(buff));
        }
      }
    }
    //Reseting actions
    for(int i = 0; i < PLAYER_COUNT; i++){
      strcpy((&p_list[i])-> action, "");
    }

    // Reset votes
    for(int i = 0; i < PLAYER_COUNT; i++){
      (&p_list[i])-> votes = 0;
    }
}

void getAction(int * socket_list, struct Player * p_list){
    int temp = fork();
    for(int i = 0; i < PLAYER_COUNT; i++){
      if (!temp){
        char input[20];
        read(socket_list[i],input,20);
        chop_space(input);
        // printf("%s action is %s\n", (&p_list[i])-> role, input);
        strcpy((&p_list[i])-> action,input);
        // printf("string length: %d\n", strlen((&p_list[i])->action));
      }
    }
  return;
}

void getWills (int * socket_list, struct Player * p_list) {
    int temp = fork();
    for(int i = 0; i < PLAYER_COUNT; i++){
      if (!temp){
        char input[100];
        read(socket_list[i],input,100);
        chop_space(input);
        strcpy((&p_list[i])-> will_statement, input);
      }
    }
  return;
}

void doActions(int * socket_list, struct Player * p_list) {
  char playerToKill[20];
  char playerToHeal[20];
  char playerToDieWill[20];
  for(int i = 0; i < PLAYER_COUNT; i++){
    char tempAction[20];
    char tempRole[20];
    strcpy(tempRole, (&p_list[i])-> role);
    strcpy(tempAction, (&p_list[i])-> action);
    if (strcmp(tempRole, "mafia") == 0){
       strcpy(playerToKill, tempAction);
    }
    if (strcmp(tempRole, "doctor") == 0){
      strcpy(playerToHeal, tempAction);
    }
  }

  printf("Who to kill: %s\n", playerToKill);
  printf("Who to heal: %s\n", playerToHeal);

  if (strcmp(playerToKill, playerToHeal) != 0){
    for (int i = 0; i < PLAYER_COUNT; i++) {
      if (strcmp(playerToKill, (&p_list[i])-> nickname) == 0){
        strcpy(playerToDieWill, (&p_list[i])-> will_statement);
      }
    }

    for(int i = 0; i < PLAYER_COUNT; i++){
      if (strcmp(playerToKill, (&p_list[i])-> nickname) == 0){
          // printf("WE HAVE FOUND HIM\n");
          char buff[250];
          strcpy(buff,"During the night, a guy killed you and you have died! \n");
          (&p_list[i])-> isalive = 0;
          write(socket_list[i],buff,sizeof(buff));
      } else {
          char buff[250];
          strcat(buff, "During the night, a guy killed ");
          strcat(buff, playerToKill);
          if (strcmp(playerToDieWill, "") == 0) {
            strcat(buff, ". He did not have a will yet." );
          } else {
            strcat(buff,"! His will was: '");
            strcat(buff, playerToDieWill);
            strcat(buff, "'.\n");
          }
          write(socket_list[i],buff,sizeof(buff));
        }
      }
  } else {
    for (int i = 0; i < PLAYER_COUNT; i++) {
      char buff[250];
      strcat(buff,"During the night, a guy tried to kill ");
      strcat(buff, playerToKill);
      strcat(buff, " but he was quickly saved by the doctor!");
      write(socket_list[i],buff,sizeof(buff));
    }
  }

  for(int i = 0; i < PLAYER_COUNT; i++){
    strcpy((&p_list[i])-> action, "");
  }
}

int verify_action(struct Player * p_list){
  int count = 0;
  for(int z = 0; z < PLAYER_COUNT; z++){
    if (strlen((&p_list[z])->action) > 0) {
      count += 1;
    }
    if ((&p_list[z])->isalive == 0) {
      count += 1;
    }
  }
  return count;
}

int verify_wills (struct Player * p_list) {
  int count = 0;
  for(int z = 0; z < PLAYER_COUNT; z++){
    if (strlen((&p_list[z])->will_statement) > 0) {
      count += 1;
    }
    if ((&p_list[z])->isalive == 0) {
      count += 1;
    }
  }
  return count;
}
