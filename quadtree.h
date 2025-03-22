#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    unsigned char red, green, blue;
    struct node *top_left, *top_right, *bot_right, *bot_left;
} TNode, *TArb;

typedef struct pixel {
    unsigned char red, green, blue;
} TPixel;

typedef struct celula {
    TArb info;
    struct celula* urm;
} TCelula, *TLista;

typedef struct coada {
    TLista inc, sf;
} TCoada;

TArb InitArb();
TCoada* InitQ () ;
TArb ExtrQ(TCoada*c, TArb *x);
int IntrQ(TCoada *c, TArb x);
void DistrQ(TCoada **c);
void DistrArb(TArb *arb);
void CompressArb(TArb *arb, TPixel **grid, int x, int y, int size, int factor);
int NrNiv(TArb arb);
int NrLeaves(TArb arb);
int MaxZone(TArb arb, int size);
void ConstrArb(TArb *arb, FILE* f);
void CompressFile(TArb arb, FILE *f);
void DecompressArb(TArb arb, TPixel ***grid, int x, int y, int size);
