struct Player {
  int socket_fd;
  int isalive;
  int perms;
  int votes;
  char nickname[20];
  char role[10];
  char action[20];
  char will_statement[100];
};
