#define MAIN
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int Min(int a , int b){
    return (a<b) ? a : b;
}

#define WORMHOLE_VALUE -10005
int Col,Row,numSnake;
#include "celle.c"

int best_index = 0;
Point best[5000];
#include "snake.c"

void InserisciMax(int val, int row, int col);

int main(int argc, char* argv[]){
    char choice = argv[1][0];
    FORESEE_DEPTH = atoi(argv[2]);
    NUM_MOSSE_CONSECUTIVE = atoi(argv[3]);
    if(NUM_MOSSE_CONSECUTIVE < 1) NUM_MOSSE_CONSECUTIVE = 1005;
    char printMat = argv[4][0];

    char inputName[] = "./inputs/0*_input.txt";
    inputName[10] = choice;
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

    wormholes = (Point*)malloc(0);
    wormholeCounter = 0;
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
            InitCella(&matrice[i][j], Value, i, j);
            if(matrice[i][j].isWormhole){
                wormholes[wormholeCounter].row = i;
                wormholes[wormholeCounter].col = j;
            }
            InserisciMax(Value, i, j);
        }
    }
    fclose(file);

    // Debug Output for the best[] array
    // for (int i = 0; i< numSnake; i++){
    //     int c,r;
    //     r = best[i].row;
    //     c = best[i].col;
    //     printf("MaxIndex: %d ; Coord: %d %d ; Val: %d\n", i, r, c, matrice[r][c].value);
    // }
    
    for (int i = 0; i< numSnake; i++){
        SetStartSnakePunto(&snakes[i], best[i]);
    }
    
    int totalScore=0;
    bool isAnySnakeLeft = true;
    while(isAnySnakeLeft){
        Snake* snake;
        isAnySnakeLeft = false;
        for(int i=0; i<numSnake; i++){
            snake = &snakes[i];
            for(int k=0; k<NUM_MOSSE_CONSECUTIVE; k++)
            if(snake->numMosse < snake->maxLength-1){
                isAnySnakeLeft = true;
                int remainingLength = snake->maxLength-1 - snake->numMosse;
                char bestMove = BestMove(snake, remainingLength>0);
                if(bestMove == 'I' || bestMove == 'W'){
                    DebugPrintMossaErrata(snake, bestMove);
                    return -1;
                }
                else{
                    //printf("Snake %d move from %d %d ", snake->index, snake->curr.row, snake->curr.col);   // DEBUG
                    Move(snake, bestMove);
                    //printf("to %d %d \n", snake->curr.row, snake->curr.col);                                // DEBUG
                }
            }
            else break;
        }
    }

    for(int i=0; i<numSnake; i++){
        totalScore += snakes[i].totalScore;
    }
    printf("Score of this run: %d\n", totalScore);
    if(choice > '1' || (printMat != 'y' && printMat != 'Y'))
        return totalScore;       // do not print matrix if too big

    printf("Printing Matrix in finish state: \n");
   
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            if(matrice[i][j].isOccupied)
                printf("%c%2d", 'X', matrice[i][j].indexOfSnake);
            else if(matrice[i][j].value == WORMHOLE_VALUE)   
                printf("%3c", '*');
            else
                printf("%3d", matrice[i][j].value);

            printf(" ");
        }
        printf("\n");
    }
    return totalScore;
}

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
