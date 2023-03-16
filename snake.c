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
char mossePossibili[] = "RLUD";
char mossaOpposta[] = "LRDU";
int FORESEE_DEPTH=1;
int NUM_MOSSE_CONSECUTIVE = 1;

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

void ApplicaMossa(Point* p, char mossa){
    switch(mossa){
            case 'U':
                p->row++;
                break;
            case 'D':
                p->row--;
                break;
            case 'R':
                p->col++;
                break;
            case 'L':
                p->col--;
                break;
            default:
                fprintf(stderr,"Mossa Errata.");
        }     
    FixIndex(p);
}
int ValoreCellaMossa(Point p, char mossa){
    ApplicaMossa(&p, mossa);
    return ValoreCella(p.row, p.col);
}
// bool isValidMove(Point p, char mossa){               Obsoleto, sostituito da ForeseePath
//     ApplicaMossa(&p, mossa);
//     return !matrice[p.row][p.col].isOccupied;
// }
void Move(Snake* snake, char mossa){
    int i = snake->numMosse;
    if(i < snake->maxLength){
        snake->numMosse++;
        snake->mosse[i] = mossa;
        ApplicaMossa(&snake->curr, mossa);
        snake->totalScore += matrice[snake->curr.row][snake->curr.col].value;
        matrice[snake->curr.row][snake->curr.col].isOccupied = true;
        matrice[snake->curr.row][snake->curr.col].indexOfSnake = snake->index;
    }
    else{
        fprintf(stderr,"Superato limite lunghezza di snake %d", snake->index);
    }
}
bool isDeadEnd(Point p){
    char mossePossibili[] = "RLUD";
    for(int i=0; i<4; i++){
        char m = mossePossibili[i]; 
        Point previewMossa = p;
        ApplicaMossa(&previewMossa, m);  
        if(matrice[previewMossa.row][previewMossa.col].isOccupied == false)
            return false;
    }
    // if all neighbours are occupied, then:
    return true;
}
bool ForeseePath(Point p, char mossa, int depth){
    ApplicaMossa(&p, mossa); 
    if(matrice[p.row][p.col].isOccupied) return false;                // caso base 1
    if(depth==0) return !matrice[p.row][p.col].isOccupied;            // caso base 2

    bool result=false;
    for(int i=0; i<4 && !result; i++){
        Point previewMossa = p;
        char m = mossePossibili[i]; 
        if(mossaOpposta[i] != mossa) {
            result = result || ForeseePath(previewMossa, m, depth-1);
        }
    }
    return result;
}
char BestMove(Snake* snake){
    Point p = snake->curr;
    int bestVal=WORMHOLE_VALUE - 100;
    char move=' ';
    for(int i=0; i<4; i++){
        char m = mossePossibili[i]; 
        int val = ValoreCellaMossa(p, m);
        bool existPath = ForeseePath(p, m, FORESEE_DEPTH);
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