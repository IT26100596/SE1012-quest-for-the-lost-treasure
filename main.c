#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size_raw 15
#define size_col 15
#define num_players 3

char map_grid[size_raw][size_col];
int hiddenTraps[size_raw][size_col] = {0};
char road[] = " ";
char wall[] = "#";

struct player {
    char name[20];
    int row;
    int col;
    int health;
    int score;
    int keys;
    char player_symbol;
};

struct player players[num_players];
int player_count = 1;


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

void printmap() {

        for(int i = 0; i < size_raw; i++) {
            
            for(int j = 0 ; j < size_col; j++) {
                printf("%c  ", map_grid[i][j]);
            }

            printf("\n");
    }
}
    

void placeWalls() {

        int wallPlaced = 0;
        
        while(wallPlaced < 30) {
            int row = 1 + rand() % 13;
            int col = 1 + rand() % 13;
            
            if(map_grid[row][col] == road[0]) {
                map_grid[row][col] = wall[0];
                wallPlaced++;
        }
        }
    }

void placeTreasures() {

    int treasurePlaced = 0;

    while(treasurePlaced < 12) {

        int row = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[row][col] == road[0] ) {
            map_grid[row][col] = 'T';
            treasurePlaced++;
        }
    }
}

void placeTraps() {
    int trapPlaced = 0;

    while(trapPlaced < 10) {

        int row = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[row][col] == road[0] && hiddenTraps[row][col] == 0) {
            hiddenTraps[row][col] = 1;
            trapPlaced++;
        }
        }
    }

void placeHealthPacks() {
    int healthPlaced = 0;

    while(healthPlaced < 5) {
        int row = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[row][col] == road[0]) {
            map_grid[row][col] = 'H';
            healthPlaced++;
        }
    }
}

void placeKeys() {
    int keyPlaced = 0;

    while(keyPlaced < 3) {
        int row = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[row][col] == road[0]) {
            map_grid[row][col] = 'K';
            keyPlaced++;
        }
    }
}

void placeDoors() {
    int doorPlaced = 0;

    while(doorPlaced < 3) {
        int row = 1 + rand() % 13;
        int col = 1 + rand() % 13;

        if(map_grid[row][col] == road[0]) {
            map_grid[row][col] = 'D';
            doorPlaced++;
        }
    }
}

void placePlayers() {

    for(int i = 0; i < player_count; i++) {

        players[i].health = 100;
        players[i].score = 0;
        players[i].keys = 0;
        players[i].player_symbol =  i + '1';

        int placed = 0; // not placed yet

        while(placed == 0) {
            int row = 1 + rand() % 13;
            int col = 1 + rand() % 13;

            if(map_grid[row][col] == ' ') {
                players[i].row = row;
                players[i].col = col;

                map_grid[row][col] = players[i].player_symbol;
                placed = 1; // plasing the player
            }
        }
    }
}

void movePlayer() {
    char move;
    int index = 0;

    printf("move(W, S, A, D): ");
    scanf(" %c", &move);

    int new_row = players[index].row;
    int new_col = players[index].col;

    if(move == 'W' || move == 'w') {
        new_row--;
    }

    else if(move == 'S' || move =='s') {
        new_row++;
    }

    else if(move == 'A' || move == 'a') {
        new_col--;
    }

    else if(move == 'D' || move == 'd') {
        new_col++;
    }

    else {
        printf("invalide move.\n");
        return;
    }

    if(map_grid[new_row][new_col] == 'D') {
        if(players[index].keys > 0) {
            players[index].keys--;
            map_grid[new_row][new_col] = road[0];
            printf("door unlocked. remaining keys: %d\n", players[index].keys);
        }

        else {
            printf("door is locked. you ran out keys.\n");
        }
    }

    if(map_grid[new_row][new_col] == '#'){
        printf("player hit a wall.\n");
        return;
    }


    map_grid[players[index].row][players[index].col] = road[0];

    players[index].row = new_row;
    players[index].col = new_col;

    processTile(index);

    map_grid[players[index].row][players[index].col] = players[index].player_symbol;
}

void processTile(int index) {

    int row = players[index].row;
    int col = players[index].col;

    if(hiddenTraps[row][col] == 1) {
        players[index].health -= 20;
        hiddenTraps[row][col] = 0;
        printf("player stepped on a trap)\n");
    }

    if(map_grid[row][col] == 'H') {
        players[index].health += 20;

        if(players[index].health > 100) {
            players[index].health = 100;
        }

        printf("player found a health pack.\n");
    }

    if(map_grid[row][col] == 'K') {
        players[index].keys++;
        printf("player found a key.\n");
    }

}

void printPlayerStatus()  {
    printf("\nplayer status: \n");
    printf("HP: %d\n", players[0].health);
    printf("Score: %d\n", players[0].score);
    printf("Keys: %d\n\n", players[0].keys);
}


int main() {

    srand(time(NULL));

    initializeMap();

    placeWalls();
    placeTreasures();
    placeTraps();
    placeHealthPacks();
    placeKeys();
    placeDoors();
    placePlayers();
    
    while (1) {
        printmap();
        printPlayerStatus();
        movePlayer();
    }

    return 0;
}

