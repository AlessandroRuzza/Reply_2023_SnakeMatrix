#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    int value;
    bool isOccupied;
    int indexOfSnake;
}Cella;

typedef struct{
    int col;
    int row;
} Point;
bool IsPointEqual(Point* a, Point* b){
    return (a->col == b->col) && (a->row == b->row);
}

int Col,Row,numSnake;
Cella matrice[5000][5000];
int best_index = 0;
Point best[5000];
#define MAIN
#define WORMHOLE_VALUE -10005
#include "snake.c"



void InserisciMax(int val, int row, int col){
    Point temp_1;
    Point temp_2;
    bool usato_1 = true;
    bool found = false;
    for(int i=0; i<numSnake; i++){
        int vBest = ValoreCellaImmediato(best[i].row, best[i].col, (best[i].row < 0 || best[i].col < 0));
        if(!found){
            if (val >= vBest){
                found = true;
                temp_1.row = best[i].row;
                temp_1.col = best[i].col;
                best[i].row = row;
                best[i].col = col;
            }
        } else {
            if (usato_1){
                temp_2.row = best[i].row;
                temp_2.col = best[i].col;
                best[i].row = temp_1.row;
                best[i].col = temp_1.col;
                usato_1 = false;
            } else {
                temp_1.row = best[i].row;
                temp_1.col = best[i].col;
                best[i].row = temp_2.row;
                best[i].col = temp_2.col;
                usato_1 = true;
            }
        }
        if(vBest < -10000)
            break;
    }
}

int main(){

    char choice;
    printf("Choose size (0-6) input?  ");
    scanf("%c", &choice);

    char inputName[] = "0*_input.txt";
    inputName[1] = choice;
    FILE *file;
    file = fopen(inputName, "r");
    
    printf("File opened.\n");

    fscanf(file, "%d", &Col);
    fscanf(file, "%d", &Row);
    fscanf(file, "%d", &numSnake);

    
    for (int i = 0; i < numSnake; i++){
        int length;
        fscanf(file, "%d", &length);
        InitSnake(&snakes[i], i, length);
        best[i].row = -1;
        best[i].col = -1;
    }

    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            char string[7];
            int Value;
            fscanf(file, "%s", string);
            if (!strcmp(string, "*")){
                Value = WORMHOLE_VALUE;
            } else {
                Value = atoi(string);
            }
            matrice[i][j].value = Value;
            matrice[i][j].isOccupied = false;
            matrice[i][j].indexOfSnake = -1;
            //printf("%d\n", Value);
            InserisciMax(Value, i, j);
        }
    }
    fclose(file);

    // Debug Output for the best[] array
    for (int i = 0; i< numSnake; i++){
        int c,r;
        r = best[i].row;
        c = best[i].col;
        printf("MaxIndex: %d ; Coord: %d %d ; Val: %d\n", i, r, c, matrice[r][c].value);
        SetStartSnakePunto(&snakes[i], best[i]);
    }
    
    int totalScore=0;
    bool isAnySnakeLeft = true;
    while(isAnySnakeLeft){
        Snake* snake;
        isAnySnakeLeft = false;
        for(int i=0; i<numSnake; i++){
            snake = &snakes[i];
            if(snake->numMosse < snake->maxLength){
                isAnySnakeLeft = true;
                char bestMove = BestMove(snake->curr);
                if(bestMove == 'I' || bestMove == 'W')
                    DebugPrintMossaErrata(snake, bestMove);
                else{
                    //printf("Snake %d move from %d %d ", snake->index, snake->curr.row, snake->curr.col);   // DEBUG
                    Move(snake, bestMove);
                    //printf("to %d %d \n", snake->curr.row, snake->curr.col);                                // DEBUG
                }
            }
        }
    }

    for(int i=0; i<numSnake; i++){
        totalScore += snakes[i].totalScore;
    }
    printf("Score of this run: %d\n", totalScore);
    if(Row > 30 || Col > 20)
        return 0;       // do not print matrix if too big


    printf("Printing Matrix in finish state: \n");
   
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            if(matrice[i][j].isOccupied)
                printf("%2c%d", 'X', matrice[i][j].indexOfSnake);
            else if(matrice[i][j].value == WORMHOLE_VALUE)   
                printf("%3c", '*');
            else
                printf("%3d", matrice[i][j].value);

            printf(" ");
        }
        printf("\n");
    }
}