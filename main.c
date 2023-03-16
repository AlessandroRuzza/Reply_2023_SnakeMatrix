#define MAIN
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int Min(int a , int b){
    return (a<b) ? a : b;
}

#define WORMHOLE_VALUE -10005
int Col,Row,numSnake;
#include "celle.c"

int best_index = 0;
Point best[5000];
#include "snake.c"

void InserisciMaxBinarySearch(int val, int row, int col);

int main(){
    char choice;
    printf("Choose size (0-6), foreseeDepth, mosseConsecutive:  ");
    scanf("%c %d %d", &choice, &FORESEE_DEPTH, &NUM_MOSSE_CONSECUTIVE);

    char inputName[] = "./inputs/0*_input.txt";
    inputName[10] = choice;
    FILE *file;
    file = fopen(inputName, "r");
    
    fscanf(file, "%d", &Col);
    fscanf(file, "%d", &Row);
    fscanf(file, "%d", &numSnake);
    
    int maxLenSnake=-1;
    for (int i = 0; i < numSnake; i++){
        int length;
        fscanf(file, "%d", &length);
        InitSnake(&snakes[i], i, length);
        if(length>maxLenSnake) maxLenSnake = length;
        best[i].row = -1;
        best[i].col = -1;
    }
    
    //wormholes = (Point*)malloc(0);
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
            else
                InserisciMaxBinarySearch(Value, i, j);
        }
    }
    fclose(file);
    printf("Input File opened and processed.\n");

    for (int i = 0; i< numSnake; i++){
        SetStartSnakePunto(&snakes[i], best[i]);
    }
    if(FORESEE_DEPTH < 0) 
        FORESEE_DEPTH=maxLenSnake;
    else
        FORESEE_DEPTH = Min(FORESEE_DEPTH, maxLenSnake);

    if(NUM_MOSSE_CONSECUTIVE < 0) 
        NUM_MOSSE_CONSECUTIVE=maxLenSnake;
    else
        NUM_MOSSE_CONSECUTIVE = Min(NUM_MOSSE_CONSECUTIVE, maxLenSnake);
    
    bool isAnySnakeLeft = true;
    while(isAnySnakeLeft){
        Snake* snake;
        isAnySnakeLeft = false;
        for(int i=0; i<numSnake; i++){
            snake = &snakes[i];
            for(int k=0; k<NUM_MOSSE_CONSECUTIVE; k++)
            if(snake->numMosse < snake->maxLength-1 && snake->isAlive){
                isAnySnakeLeft = true;
                int remainingLength = snake->maxLength-1 - snake->numMosse;
                char bestMove = BestMove(snake, remainingLength>0);
                if(bestMove == 'I' || bestMove == 'W'){
                        srand(time(NULL));
                        Point randomSpot;   
                        do{
                            randomSpot.row = rand()%Row;
                            randomSpot.col = rand()%Col;
                        }while(GetCella(randomSpot)->isOccupied);
                        RelocateSnake(snake, randomSpot);
                }
                else{
                    Move(snake, bestMove);
                }
            }
            else break;
        }
    }

    long totalScore=0;
    for(int i=0; i<numSnake; i++){
        if(snakes[i].totalScore > 0)
            totalScore += snakes[i].totalScore;
        else{ 
            snakes[i].isAlive = false;     // kills all snakes with negative score (will not put on output)
        }
    }

    printf("Score of this run: %ld\n", totalScore);

    //free(wormholes);
    return 0; 
}

void InserisciMaxBinarySearch(int val, int row, int col){
    Point temp_1;
    Point temp_2;
    int left=0;
    int right=numSnake-1;
    while(left<right){
        int mid = (left+right)/2;
        int valMid = ValoreCellaPunto(best[mid]);
        if(valMid >= val){
            left=mid+1;
        }
        else{
            right=mid;
        }
    }
    if(left > right) printf("Wow. Unexpected left>right.");
    //left == right
    if(ValoreCellaPunto(best[left]) > val)
        return;
    else{
        temp_1 = best[left];
        best[left].row = row;
        best[left].col = col;
    }
    // start propagating from the position found (==left==right)
    bool usato_1 = true;
    for(int i=left+1; i<numSnake; i++){
        if (usato_1){
            temp_2 = best[i];
            best[i] = temp_1;
            usato_1 = false;
        } else {
            temp_1 = best[i];
            best[i] = temp_2;
            usato_1 = true;
        }
        if(ValoreCellaPunto(best[i]) < WORMHOLE_VALUE) break;
    }
}
