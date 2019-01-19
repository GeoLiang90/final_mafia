# final_mafia
## Systems Final Project
## George Liang, Sajed Nahian, Richard Wong

### Description
Our project is based off of the game Mafia which consists of mafia and town members. It involves writing and storing wills each night. During the day to lynch the members of the town that could be part of the mafia. The mafia wins by eliminating innocent town members and the town wins by eliminating the mafia.

### To run the project:
- Run make <br/>
- Run ./server <br/>
- Run ./client <br/> In 3 different clients. More clients can be included by simply changing the PLAYER_COUNT variable in our code.

### Known Bugs:
- Game doesn't completely end, but people do dominate.
- Char Data corrupted from writing and reading through network sockets.
