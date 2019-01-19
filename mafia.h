#ifndef MAFIA_H
#define MAFIA_H
#include <time.h>
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int run_game(int * socket_list);
int check_win(struct Player * player_list);
void assign_roles(int * socket_list, struct Player * player_list);
void assign_name(int * socket_list, struct Player * p_list);
int verify_names(struct Player * p_list);
void chop_space(char * string);
void write_client(int * socket_list, char * buf);
void announce_executed(int * socket_list,struct Player * player_list);
extern int PLAYER_COUNT;
extern int T_WIN;
extern int  M_WIN;
extern int S_WIN;
#endif
