#ifndef MAIN
    #include "main.c"
#endif
void BackupStartValues(Snake snakesStart[], Cella matriceStart[Row][Col]){
    for (int i = 0; i < numSnake; i++){
        snakesStart[i] = snakes[i];
    }
    for (int i = 0; i < Row; i++){
        for (int j = 0; j < Col; j++){
            matriceStart[i][j] = matrice[i][j];
        }
    }
}
void PrintMatrice(Cella mat[Row][Col], int thID){
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
bool IsControlThread(Point* attr){
    if(attr->row==2 && attr->col==14){
        printf("Thread %d m=%d found error: ", attr->row, attr->col);
    }
    return (attr->row==2 && attr->col==14);
}

pthread_mutex_t stopPrint;
#define Mutex(P){\
    pthread_mutex_lock(&stopPrint);\
        P;\
    pthread_mutex_unlock(&stopPrint);\
}

long TestCase_Base(Point* attr, Snake snakesStart[], Cella matriceStart[Row][Col], bool doDebug){
    BackupStartValues(snakesStart, matriceStart);
    for (int i = 0; i < numSnake; i++){
        SetStartSnakePunto(&snakesStart[i], best[i], matriceStart);
    }
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
                if(snake->numMosse < snake->maxLength-1 && snake->isAlive){
                    isAnySnakeLeft = true;
                    int remainingLength = snake->maxLength-1 - snake->numMosse;
                    char bestMove = BestMove(snake, remainingLength>0, matriceStart, FORESEE_DEPTH);
                    if(bestMove == 'I' || bestMove == 'W'){
                        srand(time(NULL));
                        Point randomSpot;   
                        do{
                            randomSpot.row = rand()%Row;
                            randomSpot.col = rand()%Col;
                        }while(GetCella(randomSpot, matriceStart)->isOccupied);
                        RelocateSnake(snake, randomSpot, matriceStart);
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
        if(snakesStart[i].totalScore > 0)
            totalScore += snakesStart[i].totalScore;
        else{ 
            snakesStart[i].isAlive = false;     // kills all snakes with negative score (will not put on output)
        }
    }
    return totalScore;
}
void* TestCase(void* attr){
    bool doDebug = false;
    Point* p = (Point*)attr;
    
    const int snakeNum = numSnake+1;
    const int rowSize = Row;
    const int colSize = Col;
    Snake snakesStart[snakeNum];
    Cella matriceStart[rowSize][colSize];
    long bestScore = WORMHOLE_VALUE - 100;
    int bestMosse = -1;

    for(int m=0; m<=MAX_MOSSE_CONSECUTIVE; m++){
        p->col = m;
        long score = TestCase_Base(p, snakesStart, matriceStart, doDebug);
        if(score > bestScore){
            bestScore = score;
            bestMosse = m;
        }
    }
    Point* r = (Point*)malloc(sizeof(Point));
    r->row = bestScore;
    r->col = bestMosse;
    free(p);
    pthread_exit((void*)r);
}