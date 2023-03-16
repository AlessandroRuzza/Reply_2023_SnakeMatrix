#ifndef MAIN
    #include "main.c"
#endif

typedef struct{
    int index;
    int maxLength;
    Point start;
    Point curr;
    char mosse[1000];
    int numMosse; // non includo lo start
    int totalScore;
} Snake;

Snake snakes[5000];

void ApplicaMossa(Point* p, char mossa);
void CorreggiIndex(Point* p, int col, int row);

void SetStartSnake(Snake* snake, int col, int row){
    snake->start.col = col;
    snake->start.row = row;
    snake->curr.col = col;
    snake->curr.row = row;
    matrice[row][col].isOccupied = true;
    matrice[row][col].indexOfSnake = snake->index;
}
void InitSnake(Snake* snake, int index, int maxLength){
    snake->index = index;
    snake->maxLength = maxLength;
    snake->numMosse=0;
    snake->totalScore=0;
}
void SetStartSnakePunto(Snake* snake, Point p){
    SetStartSnake(snake, p.col, p.row);
}
void CopySnake(Snake* from, Snake* to){
    to->start.col = from->start.col;
    to->start.row = from->start.row;
    to->curr.col = from->curr.col;
    to->curr.row = from->curr.row;
    to->index = from->index;
    to->maxLength = from->maxLength;
    to->numMosse = from->numMosse;
    to->totalScore = from->totalScore;
}
void DebugPrintWormholeTravel(Snake* snake, Point exit){
    fprintf(stderr, "Snake %d e' entrato nel wormhole %d %d ed e' uscito nel punto %d %d.\n", 
                snake->index, snake->curr.row, snake->curr.col, exit.row, exit.col);
}
void Move(Snake* snake, char mossa, Cella matriceStart[Row+1][Col+1]){
    int i = snake->numMosse;
    if(i < snake->maxLength){
        snake->numMosse++;
        snake->mosse[i] = mossa;
        ApplicaMossa(&snake->curr, mossa);
        Cella* nextCella = GetCella(snake->curr, matriceStart);
        if(nextCella->isWormhole){
            Point exit = GetBestWormholeExit(nextCella->coords, matriceStart);
            //DebugPrintWormholeTravel(snake, exit);
            snake->curr.row = exit.row;
            snake->curr.col = exit.col;
            snake->maxLength--;
            // nextCella->isSideOccupied[GetIndexOfMossaInversa(mossa)] = true;
            nextCella = GetCella(exit, matriceStart);
        }
        nextCella->isOccupied = true;
        nextCella->indexOfSnake = snake->index;
        snake->totalScore += nextCella->value;
    }
    else{
        fprintf(stderr,"Superato limite lunghezza di snake %d", snake->index);
    }
}
bool isDeadEnd(Point p, Cella matriceStart[Row+1][Col+1]){
    for(int i=0; i<4; i++){
        char m = mossePossibili[i];
        Point previewMossa = p;
        ApplicaMossa(&previewMossa, m);  
        if(matriceStart[previewMossa.row][previewMossa.col].isOccupied == false)
            return false;
    }
    // if all neighbours are occupied, then:
    return true;
}
bool ForeseePath(Point p, char mossa, int depth, Cella matriceStart[Row+1][Col+1]){
    Cella cella = *GetCellaMossa(p, mossa, matriceStart);
    if(depth==0) return !cella.isOccupied;                                      // caso base 1
    if(cella.isOccupied) return false;                                          // caso base 2 
    // if(cella.isWormhole && !CanEnterWormholeFrom(cella, p)) return false;       // caso base 3 (wormhole)
    
    Point previewMossa;
    if(cella.isWormhole){
        previewMossa = GetBestWormholeExit(cella.coords, matriceStart);
        if(previewMossa.row == -1) return false;                                // caso base 4 (no valid exit from wormhole)
    }
    else previewMossa = cella.coords;

    bool result=false;
    for(int i=0; i<4 && !result; i++){
        char m = mossePossibili[i]; 
        if(mossaOpposta[i] != mossa){
            result = result || ForeseePath(previewMossa, m, depth-1, matriceStart);
        }
    }
    return result;
}
char BestMove(Snake* snake, bool considerWormholes, Cella matriceStart[Row+1][Col+1], int foreseeDepth){
    Point p = snake->curr;
    int bestVal=WORMHOLE_VALUE - 100;
    char move=' ';
    for(int i=0; i<4; i++){
        char m = mossePossibili[i]; 
        Cella nextCella = *GetCellaMossa(p, m, matriceStart);
        int val;
        Point wormholeExit;
        if(nextCella.isWormhole && considerWormholes){          
            wormholeExit = GetBestWormholeExit(nextCella.coords, matriceStart);
            if(wormholeExit.row == -1) 
                val = WORMHOLE_VALUE-100;
            else
                val = ValoreCellaPunto(wormholeExit, matriceStart);
        }
        else val = nextCella.value;     // if on wormhole and not considering wormholes, val = WORMHOLE_VALUE
        int remainingLength = snake->maxLength-1 - snake->numMosse;
        bool existPath = ForeseePath(p, m, Min(foreseeDepth, remainingLength-1), matriceStart);
        if(val>bestVal && existPath){
            bestVal=val;
            move=m;
        }
    }

    if(bestVal == WORMHOLE_VALUE){
        return 'W';         // W = "wormhole"
    }
    if(move == ' '){
        return 'I';        // I = "intrappolato"
    }

    return move;
}
void DebugPrintMossaErrata(Snake* snake, char mossaErrata){
    fprintf(stderr, "Snake %d nel punto %d %d ha avuto errore di tipo: ", snake->index, snake->curr.row, snake->curr.col);
    if(mossaErrata == 'I'){
        // debug per lo snake intrappolato
        fprintf(stderr,"SNAKE_INTRAPPOLATO\n");
    }
    if(mossaErrata == 'W'){
        // debug per lo snake nel wormhole
        fprintf(stderr,"SNAKE_WORMHOLE\n");
    }
    snake->maxLength = snake->numMosse; // "kills" snake
}