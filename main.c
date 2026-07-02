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

void printMap() {

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
            int raw = 1 + rand() % 13;
            int col = 1 + rand() % 13;
            
            if(map_grid[raw][col] == road[0]) {
                map_grid[raw][col] = wall[0];
                wallPlaced++;
        }
        }
    }

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

void processTile(int index) {
    int raw = players[index].raw;
    int col = players[index].col;

    if(hiddenTraps[raw][col] == 1) {
        players[index].health -= 20;
        hiddenTraps[raw][col] = 0;
        printf("player stepped on a trap.\n");
    }

    if(map_grid[raw][col] == 'T') {
        players[index].score += 10;
        printf("player found a treasre.\n");
    }

    if(map_grid[raw][col] == 'H') {
        players[index].health += 20;

        if(players[index].health > 100) {
            players[index].health = 100;
        }

        printf("player found a health pack.\n");
    }

    if(map_grid[raw][col] == 'K') {
        players[index].keys++;
        printf("player found a key.\n");
    }

}

int isValidMove(int raw, int col) {

    if(raw < 0 || raw >= size_raw || col < 0 || col >= size_col) {
        return 0;
    }
    if(map_grid[raw][col] == '#') {
        return 0;
    }

    return 1;
}

void movePlayer(int index) {
    char move[10];

    printf("move(W, S, A, D): ");
    scanf(" %s", move);

    if(strlen(move) > 4) {
        printf("invalid move.step Kipped.\n");
        return;
    }

    for(int i = 0; i <strlen(move); i++) {
        int new_raw = players[index].raw;
        int new_col = players[index].col;

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

        if(map_grid[new_raw][new_col] == 'D') {
            if(players[index].keys > 0) {
                players[index].keys--;
                map_grid[new_raw][new_col] = road[0];
                printf("door unlocked. remaining keys: %d\n", players[index].keys);
            }
            else{
                printf("door is locked. you dont have a key.\n");
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
    printf("\n");

    while (1) {
        printMap();
        printPlayerStatus();
        movePlayer(0);
        printf("\n");
    }

    return 0;

}

