struct Player {
  int socket_fd;
  int isalive;
  int perms;
  char nickname[20];
  char role[10];
  char will_statement[100];
};
