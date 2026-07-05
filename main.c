#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define size_raw 15
#define size_col 15
#define num_players 3

char map_grid[size_raw][size_col];
int hiddenTraps[size_raw][size_col] = {0};
char road[] = " ";
char wall[] = "#";
char gameLogs[50][100];
int logCount = 0;

int remainingTreasures();

struct player {
    char name[20];
    int raw;
    int col;
    int health;
    int score;
    int keys;
    char player_symbol;
};

struct player players[num_players];
int player_count = 1;
int total_treasures = 12;
int collected_treasures = 0;
int game_over = 0;

//map boundry
void initializeMap() {

        for(int i = 0; i < size_raw; i++) {

            for(int j = 0; j < size_col; j++) {

                if(i == 0 || j == 0 || i == size_raw - 1 || j == size_col - 1)
                map_grid[i][j] = wall[0];

                else
                map_grid[i][j] = road[0];
            }
        }
    }

//print map
void printMap() {

        for(int i = 0; i < size_raw; i++) {
            
            for(int j = 0 ; j < size_col; j++) {
                printf("%c  ", map_grid[i][j]);
            }

            printf("\n");
    }
}

//placing the walls
void placeWalls() {

        int wallPlaced = 0;
        
        while(wallPlaced < 30) {
            int raw = 1 + rand() % 13;
            int col = 1 + rand() % 13;
            
            if(map_grid[raw][col] == road[0]) {
                map_grid[raw][col] = wall[0];
                wallPlaced++;
        }
        }
    }
    
//placing the treasure
void placeTreasures() {

    int treasurePlaced = 0;

    while(treasurePlaced < 12) {

        int raw = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[raw][col] == road[0] ) {
            map_grid[raw][col] = 'T';
            treasurePlaced++;
        }
    }
}

//placing random traps
void placeTraps() {
    int trapPlaced = 0;

    while(trapPlaced < 10) {

        int raw = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[raw][col] == road[0] && hiddenTraps[raw][col] == 0) {
            hiddenTraps[raw][col] = 1;
            trapPlaced++;
        }
        }
    }
    
//placing the health packs
void placeHealthPacks() {
    int healthPlaced = 0;

    while(healthPlaced < 5) {
        int raw = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[raw][col] == road[0]) {
            map_grid[raw][col] = 'H';
            healthPlaced++;
        }
    }
}

//placing the keys
void placeKeys() {
    int keyPlaced = 0;

    while(keyPlaced < 3) {
        int raw = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[raw][col] == road[0]) {
            map_grid[raw][col] = 'K';
            keyPlaced++;
        }
    }
}

//placing the doors
void placeDoors() {
    int doorPlaced = 0;

    while(doorPlaced < 3) {
        int raw = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[raw][col] == road[0]) {
            map_grid[raw][col] = 'D';
            doorPlaced++;
        }
    }
}

// setting up players
void setupPlayers() {

    do {
        printf("\nenter number of players: ");
        scanf("%d", &player_count);
        printf("\n");

        if(player_count < 1 || player_count > 3) {
            printf("invalid number of players. enter again.\n");
        }
    }
    while(player_count < 1 || player_count > 3);

    for(int i = 0; i < player_count; i++) {
        printf("enter name for player %d: ", i + 1);
        scanf("%s", players[i].name);
    }
}

//placing the players on map
void placePlayers() {

    for(int i = 0; i < player_count; i++) {

        players[i].health = 100;
        players[i].score = 0;
        players[i].keys = 0;
        players[i].player_symbol =  i + '1';

        int placed = 0; // not placed yet

        while(placed == 0) {
            int raw = 1 + rand() % 13;
            int col = 1 + rand() % 13;

            if(map_grid[raw][col] == ' ') {
                players[i].raw = raw;
                players[i].col = col;

                map_grid[raw][col] = players[i].player_symbol;
                placed = 1; // plasing the player
            }
        }
    }
}

//function to print recent game log
void printRecentLog() {
    printf("\n---------- game log ----------\n");

    if(logCount == 0) {
        printf("no events for now.\n");
    }
    else {
        for(int i = 0; i < logCount; i++) {
            printf("%d. %s\n", i + 1, gameLogs[i]);
        }
    }

    printf("--------------------\n");
}

//funtion to save log
void saveLog() {
    FILE *file = fopen("gamelog.txt", "w");

    if(file == NULL) {
        printf("Could not save game log.\n");
        return;
    }

    for(int i = 0; i < logCount; i++) {
        fprintf(file, "%d. %s\n", i + 1, gameLogs[i]);
    }

    fclose(file);
    printf("Game log saved to gamelog.txt\n");
}

//save whole game
void saveGame() {
    FILE *file = fopen("savegame.dat", "wb");

    if(file == NULL) {
        printf("Could not save game.\n");
        return;
    }

    fwrite(&player_count, sizeof(int), 1, file);
    fwrite(players, sizeof(struct player), num_players, file);
    fwrite(map_grid, sizeof(char), size_raw * size_col, file);
    fwrite(hiddenTraps, sizeof(int), size_raw * size_col, file);
    fwrite(&collected_treasures, sizeof(int), 1, file);

    fclose(file);

    printf("Game saved to savegame.dat\n");
}

//start game from recently stoped place
void loadGame() {
    FILE *file = fopen("savegame.dat", "rb");

    if(file == NULL) {
        printf("No saved game found.\n");
        return;
    }

    fread(&player_count, sizeof(int), 1, file);
    fread(players, sizeof(struct player), num_players, file);
    fread(map_grid, sizeof(char), size_raw * size_col, file);
    fread(hiddenTraps, sizeof(int), size_raw * size_col, file);
    fread(&collected_treasures, sizeof(int), 1, file);

    fclose(file);

    printf("Game loaded from savegame.dat\n");
}


void addLog(char message[]) {
    if(logCount < 50) {
        strcpy(gameLogs[logCount], message);
        logCount++;
    }
}

// check players status on each tile and print
void processTile(int index) {
    int raw = players[index].raw;
    int col = players[index].col;

    if(hiddenTraps[raw][col] == 1) {
        players[index].health -= 20;
        hiddenTraps[raw][col] = 0;

        if(players[index].health < 0) {
            players[index].health = 0;
        }
        printf("player stepped on a trap.\n");
        addLog("player stepped on a trap.");
       
    }

    if(map_grid[raw][col] == 'T') {
        players[index].score += 10;
        collected_treasures++;
        map_grid[raw][col] = road[0];

        printf("player found a treasure.\n");
        addLog("player found a treasure.");

        if(remainingTreasures() == 0) {
            printf("\nAll treasures collected. you win!\n");
            game_over = 1;
        }   
    }

    else if(map_grid[raw][col] == 'H') {
        players[index].health += 20;

        if(players[index].health > 100) {
            players[index].health = 100;
        }
        map_grid[raw][col] = road[0];

        printf("player found a health pack.\n");
        addLog("player found a health pack.");
    }

    else if(map_grid[raw][col] == 'K') {
        players[index].keys++;
        map_grid[raw][col] = road[0];

        printf("player found a key.\n");
        addLog("A player found a key.");
    }

}


int isPlayerThere(int raw, int col) {
    for(int i = 0; i < player_count; i++) {
        if(players[i].raw == raw && players[i].col == col && players[i].health > 0) {
            printf("player %s is here.\n", players[i].name);
            return 1;
        }
    }
    return 0;
}

//checking the inputs are valid moves
int isValidMove(int raw, int col) {

    if(raw < 0 || raw >= size_raw || col < 0 || col >= size_col) {
        return 0;
    }
    if(map_grid[raw][col] == '#') {
        return 0;
    }

    return 1;
}

//moving player through map
void movePlayer(int index) {
    char move[10];

    printf("enter move - W, S, A, D, P to save, or Q to quit: ");
    scanf(" %s", move);

    if(strlen(move) > 4) {
        printf("invalid move.step Kipped.\n");
        return;
    }

    for(int i = 0; i < strlen(move); i++) {
        int new_raw = players[index].raw;
        int new_col = players[index].col;

        if(move[i] == 'P' || move[i] == 'p') {
            saveGame();
            continue;
        }

        if(move[i] == 'Q' || move[i] =='q') {
            printf("player quit the game.\n");
            game_over = 1;
            return;
        }

        if(move[i] == 'W' || move[i] == 'w') {
            new_raw--;
        }

        else if(move[i] == 'S' || move[i] == 's') {
            new_raw++;
        }

        else if(move[i] == 'A' || move[i] == 'a') {
            new_col--;
        }

        else if(move[i] == 'D' || move[i] == 'd') {
            new_col++;
        }

        else {
            printf("invalid move.step skipped.\n");
            continue;
        }

        if(isValidMove(new_raw, new_col) == 0) {
            printf("Hit a wall. step skipped.\n");
            continue;
        }

        if(isPlayerThere(new_raw, new_col) == 1) {
            printf("another player is there. step skipped.\n");
            continue;
        }

        if(map_grid[new_raw][new_col] == 'D') {
            if(players[index].keys > 0) {
                players[index].keys--;
                map_grid[new_raw][new_col] = road[0];
                printf("door unlocked. remaining keys: %d\n", players[index].keys);
                addLog("door was unlocked.");
            }
            else{
                printf("door is locked. you dont have a key.\n");
                addLog("player tried to open a locked door.");
                continue;
            }
        }

        map_grid[players[index].raw][players[index].col] = road[0];
        players[index].raw = new_raw;
        players[index].col = new_col;

        processTile(index);

        map_grid[players[index].raw][players[index].col] = players[index].player_symbol;
    }
}

//remaining treasures to coll
int remainingTreasures() {
    int count = 0;

    for(int i = 0; i < size_raw; i++) {
        for(int j = 0; j < size_col; j++) {
            if(map_grid[i][j] == 'T') {
                count++;
            }
        }
    }

    return count;
}

//showing each player scores
void showScore() {
    int winner = 0;

    for(int i = 1; i < player_count; i++) {
        if(players[i].score > players[winner].score) {
            winner = i;
        }
    }
    printf("\n>>>>>>>>>> Score Board <<<<<<<<<<\n\n");

    for(int i = 0; i < player_count; i++) {
        printf("player %d (%s):   | HP: %d    | Score: %d   | Keys: %d\n\n", i + 1, players[i].name, players[i].health, players[i].score, players[i].keys);
    }

    printf("Winner is player: %s", players[winner].name);

    printf("\n\n>>>>>>>>>> Game Over <<<<<<<<<<\n\n");
}

//showing each player statistic
void showStats() {
    int alivePlayer = 0;
    int failedPlayer = 0;

    for(int i = 0; i < player_count; i++) {
        if(players[i].health > 0) {
            alivePlayer++;
        }
        else {
            failedPlayer++;
        }
    }

    printf("\n>>>>>>>>>> Game Stats <<<<<<<<<<\n\n");
    printf("Total players: %d\n", player_count);
    printf("Alive Players: %d\n", alivePlayer);
    printf("Failed Players: %d\n", failedPlayer);
    printf("Treasures collected: %d\n", collected_treasures);
    printf("Treasures remaining: %d\n", remainingTreasures());
    printf("------------------------------\n");
}

//function to print each player status
void printPlayerStatus()  {
    printf("\nplayer status: \n\n");

    for(int i = 0; i < player_count; i++)  {
        printf("player %d (%s):   | HP: %d    | Score: %d   | Keys: %d\n\n",
               i + 1, players[i].name, players[i].health, players[i].score, players[i].keys);
    }

    printf("Treasures remaining: %d/%d\n\n", remainingTreasures(), total_treasures);
}

//function to if all players failed in the middle of the game
int all_players_failed() {
    for(int i = 0; i < player_count; i++) {
        if(players[i].health > 0) {
            return 0;
        }
    }

    return 1;
}

//game main logic
void gameLoop() {
    while (game_over == 0) {
        printMap();
        printPlayerStatus();

        for(int i = 0; i < player_count; i++) {
            if(players[i].health > 0) {
                printf("\nplayer %s turn >>>\n\n", players[i].name);
                movePlayer(i);

                if(game_over == 1) {
                    break;
                }
            }
            else {
                printf("\nplayer %s is failed. next player turn >>>\n", players[i].name);
            }
        }

        if(all_players_failed() == 1) {
            printf("\nAll players failed. game over.\n");
            game_over = 1;
        }
    }

    showScore();
    showStats();
    printRecentLog();
    saveLog();
    
}

//calling functions
int main() {

    srand(time(NULL));

    int choice;
    printf("\n1. New Game\n");
    printf("2. Load Game\n");
    printf("\nEnter choice: ");
    scanf("%d", &choice);
    
    if(choice == 2) {
        loadGame();
        gameLoop();
        return 0;
    }
    
   initializeMap();

    placeWalls();
    placeTreasures();
    placeTraps();
    placeHealthPacks();
    placeKeys();
    placeDoors();
    setupPlayers();
    placePlayers();
    printf("\n");

    gameLoop();

    return 0;

}
