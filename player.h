struct Player {
  int socket_fd;
  int isalive;
  int perms;
  int votes;
  int executed;
  int exec_a;
  int announced;
  char nickname[20];
  char role[10];
  char action[20];
  char will_statement[100];
};
