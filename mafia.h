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
#define PLAYER_COUNT 6
#define T_WIN 1
#define M_WIN 2
#define S_WIN 3
#endif
