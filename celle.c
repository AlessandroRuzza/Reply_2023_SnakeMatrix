#ifndef MAIN
    #include "main.c"
#endif

typedef struct{
    int col;
    int row;
} Point;
char mossePossibili[] = "RLUD";
char mossaOpposta[] = "LRDU";

bool IsPointEqual(Point* a, Point* b){
    return (a->col == b->col) && (a->row == b->row);
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
char FindMossa(Point* from, Point* to){
    char mossa = ' ';
    for(int i=0; i<4 && mossa==' '; i++){
        char m = mossePossibili[i];
        Point p = *from;
        ApplicaMossa(&p, m);
        if(IsPointEqual(&p, to))
            mossa = m;
    }
    if(mossa == ' ') return 'E';    // 'E' as in "Error"
    else return mossa;
}
int GetIndexOfMossa(char mossa){
    if(mossa == 'R') return 0;
    if(mossa == 'L') return 1;
    if(mossa == 'U') return 2;
    if(mossa == 'D') return 3;
    else return -1;
}
int GetIndexOfMossaInversa(char mossa){
    if(mossa == 'L') return 0;
    if(mossa == 'R') return 1;
    if(mossa == 'D') return 2;
    if(mossa == 'U') return 3;
    else return -1;
}

typedef struct{
    Point coords;
    int value;
    bool isOccupied;
    int indexOfSnake;
    bool isWormhole;
}Cella;

Cella matrice[5000][5000];
Point wormholes[5000*1000];
int wormholeCounter;

void InitCella(Cella* cella, int value, int row, int col){
    cella->coords.row = row;
    cella->coords.col = col;
    cella->value = value;
    cella->isOccupied = false;
    cella->indexOfSnake = -1;
    cella->isWormhole = (value == WORMHOLE_VALUE);

    if(cella->isWormhole){
        wormholeCounter++;
        //size_t newSize = wormholeCounter*sizeof(Point);
        //wormholes = (Point*)realloc(wormholes, newSize);
        wormholes[wormholeCounter-1] = cella->coords;
    }
}
Cella* GetCellaMossa(Point from, char mossa){
    ApplicaMossa(&from, mossa);
    return &matrice[from.row][from.col];
}
Cella* GetCella(Point p){
    return &matrice[p.row][p.col];
}
Point GetBestWormholeExit(Point wormhole){
    Cella bestExit;
    InitCella(&bestExit, WORMHOLE_VALUE-10, -1, -1);
    for(int i=0; i<wormholeCounter; i++){
        if(!IsPointEqual(&wormhole, &wormholes[i]))
        for(int k=0; k<4; k++){
            char m = mossePossibili[k];
            Cella exit = *GetCellaMossa(wormholes[i], m);
            if(exit.value > bestExit.value && !exit.isOccupied) 
                bestExit = exit;
        }
    }

    if(bestExit.value > WORMHOLE_VALUE)
        return bestExit.coords;
    else{
        Point errorCase;
        errorCase.row = -1; errorCase.col = -1;
        return errorCase;
    }

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
int ValoreCellaPunto(Point p){
    if(p.row < 0 || p.col < 0) 
        return WORMHOLE_VALUE - 100;
    else 
        return matrice[p.row][p.col].value;
}

