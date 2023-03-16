#define MAIN
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

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

#include "test.c"
pthread_t threads[1000][1000];

int main(){
    char choice;
    int MAX_FORESEE_DEPTH;
    int MAX_MOSSE_CONSECUTIVE;
    printf("Choose size (0-6), MAX_FORESEE_DEPTH, MAX_MOSSE_CONSECUTIVE:  ");
    scanf("%c %d %d", &choice, &MAX_FORESEE_DEPTH, &MAX_MOSSE_CONSECUTIVE);
    
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
    
    printf("Input File opened and processed.\n");

    for (int i = 0; i< numSnake; i++){
        SetStartSnakePunto(&snakes[i], best[i]);
    }

    long bestScore=WORMHOLE_VALUE - 100;
    int bestForesee=-1;
    int bestMosse=-1;
    
    if(MAX_FORESEE_DEPTH < 0) 
        MAX_FORESEE_DEPTH=maxLenSnake;
    else
        MAX_FORESEE_DEPTH = Min(MAX_FORESEE_DEPTH, maxLenSnake);

    if(MAX_MOSSE_CONSECUTIVE < 0) 
        MAX_MOSSE_CONSECUTIVE=maxLenSnake;
    else
        MAX_MOSSE_CONSECUTIVE = Min(MAX_MOSSE_CONSECUTIVE, maxLenSnake);

    int totThreads=0;
    Point* p;
    pthread_mutex_init(&stopPrint, NULL);
    pthread_mutex_lock(&stopPrint);
        for(int f=0; f<=MAX_FORESEE_DEPTH; f++){
            for(int m=0; m<=MAX_MOSSE_CONSECUTIVE; m++){
                p = NULL;
                p = malloc(sizeof(Point));
                p->row = f;
                p->col = m;
                p->threadID = totThreads;
                pthread_attr_t attr;
                pthread_attr_init(&attr);
                pthread_attr_setstacksize(&attr, sizeof(Cella)*(Row+2)*(Col+2) + sizeof(Snake)*(numSnake+2));
                int flag = pthread_create(&threads[f][m], &attr, TestCase, p);

                if(flag != 0) fprintf(stderr, "Problem with thread %d %d ; returned %d", f, m, flag);
                else totThreads++;
            }
        }
        printf("All %d threads created.\n", totThreads);
    pthread_mutex_unlock(&stopPrint);

    for(int f=0; f<=MAX_FORESEE_DEPTH; f++){
        for(int m=0; m<=MAX_MOSSE_CONSECUTIVE; m++){
            void* scoreP;
            long score;
            printf("Waiting for Thread %d %d to join... ", f, m);
            int flag = pthread_join(threads[f][m], &scoreP);
            if(flag != 0) fprintf(stderr, "Problem with thread %d %d ; returned %d", f, m, flag);
            score = *(long*)scoreP;
            printf("Thread %d %d joined!\n", f, m);
            if(score > bestScore){
                bestScore = score;
                bestForesee = f;
                bestMosse = m;
            }
            if(score > 0){
                printf("Positive %ld result with params: F=%d M=%d\n", score, f, m);
            }
        }
    }

    printf("\nMax Score reached: %ld with params: F=%d M=%d\n", bestScore, bestForesee, bestMosse);
    printf("Max Params tested: F=%d M=%d", MAX_FORESEE_DEPTH, MAX_MOSSE_CONSECUTIVE);
    printf("\a");
    free(wormholes);
    return bestScore;
}

void InserisciMax(int val, int row, int col){
    Point temp_1;
    Point temp_2;
    bool usato_1 = true;
    bool found = false;
    for(int i=0; i<numSnake; i++){
        int vBest = ValoreCellaImmediato(best[i].row, best[i].col, (best[i].row < 0 || best[i].col < 0), matrice);
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
