
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

void CorreggiIndex(Point* returnPoint, int col, int row){
    if(col>=Col){
        col -= Col;
    }
    if(row>=Row){
        row -= Row;
    }
    if(col<0){
        col += Col;
    }
    if(row<0){
        row += Row;
    }
    returnPoint->col = col;
    returnPoint->row = row;
}
void FixIndex(Point* p){
    CorreggiIndex(p, p->col, p->row);
}

int ValoreCella(int row, int col){ 
    Point p;
    CorreggiIndex(&p, col, row);
    return matrice[p.row][p.col].value;
}
int ValoreCellaMossa(Point p, char mossa){
    ApplicaMossa(&p, mossa);
    return ValoreCella(p.row, p.col);
}
int ValoreCellaImmediato(int row, int col, bool t){
    if(t)
        return -10002;
    else
        return matrice[row][col].value;
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
bool isValidMove(Point p, char mossa){
    ApplicaMossa(&p, mossa);
    return !matrice[p.row][p.col].isOccupied;
}
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

char BestMove(Point p){
    int bestVal=WORMHOLE_VALUE - 100;
    char move=' ';
    int val = ValoreCellaMossa(p, 'R');
    if(isValidMove(p, 'R')){
        bestVal=val;
        move='R';
    }
    val = ValoreCellaMossa(p, 'L');
    if(val>bestVal && isValidMove(p, 'L')){
        bestVal=val;
        move='L';
    }
    val = ValoreCellaMossa(p, 'U');
    if(val>bestVal && isValidMove(p, 'U')){
        bestVal=val;
        move='U';
    }
    val = ValoreCellaMossa(p, 'D');
    if(val>bestVal && isValidMove(p, 'D')){
        bestVal=val;
        move='D';
    }

    if(bestVal == WORMHOLE_VALUE){
        return 'W';
    }
    if(move == ' '){
        return 'I';
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
