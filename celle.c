#ifndef MAIN
    #include "main.c"
#endif

typedef struct{
    int value;
    bool isOccupied;
    int indexOfSnake;
    bool isWormhole;
    bool isSideOccupied[4];
}Cella;

Cella matrice[5000][5000];
void InitCella(Cella* cella, int value){
    cella->value = value;
    cella->isOccupied = false;
    cella->indexOfSnake = -1;
    cella->isWormhole = (value == WORMHOLE_VALUE);
    cella->isSideOccupied[0] = false;
    cella->isSideOccupied[1] = false;
    cella->isSideOccupied[2] = false;
    cella->isSideOccupied[3] = false;
}

typedef struct{
    int col;
    int row;
} Point;
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

int ValoreCellaImmediato(int row, int col, bool t){
    if(t)
        return -10002;
    else
        return matrice[row][col].value;
}
int ValoreCella(int row, int col){ 
    Point p;
    CorreggiIndex(&p, col, row);
    return matrice[p.row][p.col].value;
}