#ifndef MAIN
    #include "main.c"
#endif

Snake snakesStart[5000];
Cella matriceStart[5000][5000];
void BackupStartValues(){
    for (int i = 0; i < numSnake; i++){
        snakesStart[i] = snakes[i];
    }
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            matriceStart[i][j] = matrice[i][j];
        }
    }
}
void RestoreSnakes(){
    for (int i = 0; i < numSnake; i++){
        snakes[i] = snakesStart[i];
    }
}
void RestoreMatrice(){
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            matrice[i][j] = matriceStart[i][j];
        }
    }
}

long TestCase(int f, int m, bool doDebug){
    RestoreSnakes();
    RestoreMatrice();
    FORESEE_DEPTH = f;
    NUM_MOSSE_CONSECUTIVE = m;
    if(NUM_MOSSE_CONSECUTIVE < 1) NUM_MOSSE_CONSECUTIVE = 1005;
    long totalScore=0;
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
                    if(doDebug) DebugPrintMossaErrata(snake, bestMove);
                    return WORMHOLE_VALUE - 100;
                }
                else{
                    Move(snake, bestMove);
                }
            }
            else break;
        }
    }

    for(int i=0; i<numSnake; i++){
        totalScore += snakes[i].totalScore;
    }
    //printf("Score of this run: %d ; with Params F=%d M=%d\n", totalScore, f, m);
    return totalScore;
}