struct Player {
  int socket_fd;
  int isalive;
  char nickname[20];
  char role[10];
  char will_statement[100];
};
