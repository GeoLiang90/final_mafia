#ifndef MAFIA_H
#define MAFIA_H
#include <time.h>
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
int run_game(int * socket_list);
int check_win(struct Player * player_list);
void assign_roles(int * socket_list, struct Player * player_list);
const int PLAYER_COUNT = 3;
const int T_WIN = 1;
const int  M_WIN  = 2;
const int S_WIN = 3;
#endif
