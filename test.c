#ifndef MAIN
    #include "main.c"
#endif
void BackupStartValues(Snake snakesStart[], Cella matriceStart[Row+1][Col+1]){
    for (int i = 0; i < numSnake; i++){
        snakesStart[i] = snakes[i];
    }
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            matriceStart[i][j] = matrice[i][j];
        }
    }
}
void PrintMatrice(Cella mat[Row+1][Col+1], int thID){
    printf("Print matrice for thread %d: \n", thID);
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            if(mat[i][j].isOccupied)
                printf("%2d%c", mat[i][j].indexOfSnake, 'X');
            else if(mat[i][j].value == WORMHOLE_VALUE)   
                printf("%3c", '*');
            else
                printf("%3d", mat[i][j].value);

            printf(" ");
        }
        printf("\n");
    }
}
void PrintAdvancement(int thID, int step, char* state){
    printf("Thread %d arrived to step %s #%d\n", thID, state, step);
}
pthread_mutex_t stopPrint;
long TestCase_Base(Point* attr, bool doDebug){
    const int snakeNum = numSnake+1;
    const int rowSize = Row+1;
    const int colSize = Col+1;
    Snake snakesStart[snakeNum];
    Cella matriceStart[rowSize][colSize];
    BackupStartValues(snakesStart, matriceStart);
    pthread_mutex_lock(&stopPrint);
        //PrintMatrice(matriceStart, attr->threadID);
        //printf("\n\n");
    pthread_mutex_unlock(&stopPrint);
    int FORESEE_DEPTH = attr->row;
    int NUM_MOSSE_CONSECUTIVE = attr->col;
    if(NUM_MOSSE_CONSECUTIVE < 1) NUM_MOSSE_CONSECUTIVE = 1005;

    long totalScore=0;
    bool isAnySnakeLeft = true;
    while(isAnySnakeLeft){
        Snake* snake;
        isAnySnakeLeft = false;
        for(int i=0; i<numSnake; i++){
            snake = &snakesStart[i];
            for(int k=0; k<NUM_MOSSE_CONSECUTIVE; k++){
                if(snake->numMosse < snake->maxLength-1){
                    isAnySnakeLeft = true;
                    int remainingLength = snake->maxLength-1 - snake->numMosse;
                    char bestMove = BestMove(snake, remainingLength>0, matriceStart, FORESEE_DEPTH);
                    if(bestMove == 'I' || bestMove == 'W'){
                        if(doDebug) DebugPrintMossaErrata(snake, bestMove);
                        return WORMHOLE_VALUE - 100;
                    }
                    else{
                        Move(snake, bestMove, matriceStart);
                    }
                }
                else break;
            }
        }
    }
        
    for(int i=0; i<numSnake; i++){
        totalScore += snakesStart[i].totalScore;
    }
    return totalScore;
}
void* TestCase(void* attr){
    bool doDebug = false;
    Point* p = (Point*)attr;
    long* r = (long*)malloc(sizeof(long));
    *r = TestCase_Base(p, doDebug);
    pthread_exit((void*)r);
}