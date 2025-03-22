#include "quadtree.h"

/* Functie care initializeaza un nod de arbore */
/* Function that initializes a tree node */
TArb InitArb() {
    TArb arb = (TArb)malloc(sizeof(TNode));
    if (!arb) {
        return NULL;
    }
    arb->red = 0;
    arb->green = 0;
    arb->blue = 0;
    arb->top_left = NULL;
    arb->top_right = NULL;
    arb->bot_left = NULL;
    arb->bot_right = NULL;
    return arb;
}

/* Functie de distrugere arbore */
/* Function that destroys the tree */
void DistrArb(TArb *arb)
{
    if((*arb)->top_left != NULL && (*arb)->top_left != NULL && (*arb)->top_left != NULL && (*arb)->top_left != NULL) {
		DistrArb(&(*arb)->top_left);
		DistrArb(&(*arb)->top_right);
		DistrArb(&(*arb)->bot_right);
	    DistrArb(&(*arb)->bot_left);
	    }
	free((*arb));
}

/* Functie care construieste arborele pe baza fisierului PPM*/
/* Function that builds the tree based on the PPM file */
void CompressArb(TArb *arb, TPixel **grid, int x, int y, int size, int factor) {
    *arb = InitArb();
    unsigned long long int red = 0, green = 0, blue = 0, mean = 0;
    int i = 0, j = 0;

    // the average is calculated for each color
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            red = red + grid[i][j].red;
            green = green + grid[i][j].green;
            blue = blue + grid[i][j].blue;
        }
    }
    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);
    (*arb)->red = red;
    (*arb)->green = green;
    (*arb)->blue = blue;

    // the similarity score of the pixels is calculated (mean)
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            mean = mean + (red - grid[i][j].red) * (red - grid[i][j].red) +
                    (green - grid[i][j].green) * (green - grid[i][j].green) +
                    (blue - grid[i][j].blue) * (blue - grid[i][j].blue);
        }
    }
    mean = mean / (3 * size * size);
    if (mean > factor) {
        // the similarity score of the pixels is too high
        // -> recursive call (the node is divided)
        CompressArb(&(*arb)->top_left, grid, x, y, size / 2, factor);
        CompressArb(&(*arb)->top_right, grid, x, y + (size / 2), size / 2, factor);
        CompressArb(&(*arb)->bot_right, grid, x + (size / 2), y + (size / 2), size / 2, factor);
        CompressArb(&(*arb)->bot_left, grid, x + (size / 2), y, size / 2, factor);
        return;
    } else if (mean <= factor) {
        // the similarity score of the pixels is good
        // the node becomes a leaf
        (*arb)->top_left = NULL;
        (*arb)->top_right = NULL;
        (*arb)->bot_right = NULL;
        (*arb)->bot_left = NULL;
        return;
    }
}

/* Functie care returneaza numarul de niveluri din arbore */
/* Function that returns the number of levels in the tree */
int NrNiv(TArb arb) {
    int ntl, ntr, nbl, nbr, max;
    if (!arb) {
        return 0;
    }
    ntl = NrNiv(arb->top_left);
    ntr = NrNiv(arb->top_right);
    nbl = NrNiv(arb->bot_left);
    nbr = NrNiv(arb->bot_right);
    if (ntl >= ntr && ntl >= nbl && ntl >= nbr) {
        max = ntl;
    } else if (ntr >= ntl && ntr >= nbl && ntr >= nbr) {
        max = ntr;
    } else if (nbl >= ntl && nbl >= ntr && nbl >= nbr) {
        max = nbl;
    } else {
        max = nbr;
    }
    return 1 + max;
}

/* Functie care returneaza numarul de frunze din arbore */
/* Function that returns the number of leaves in the tree */
int NrLeaves(TArb arb) {
    int count = 0;
    if (!arb) {
        return 0;
    }
    count += NrLeaves(arb->top_left);
    count += NrLeaves(arb->top_right);
    count += NrLeaves(arb->bot_right);
    count += NrLeaves(arb->bot_left);
    if (!arb->top_left && !arb->top_right && !arb->bot_left && !arb->bot_right) {
        count += 1;
    }
    return count;
}

/* Functie care returneaza lungimea celui mai mare patrat care nu se divide */
/* Function that returns the length of the largest square that is not divided */
int MaxZone(TArb arb, int size) {
    int ztl, ztr, zbl, zbr, max;
    if (!arb) {
        return 0;
    }
    if (!arb->top_left && !arb->top_right && !arb->bot_left && !arb->bot_right) {
        // if it's just the root, the length is the initial length
        return size;
    } else {
        // otherwise, it is called recursively with the length / 2 (the node is divided)
        ztl = MaxZone(arb->top_left, size / 2);
        ztr = MaxZone(arb->top_right, size / 2);
        zbr = MaxZone(arb->bot_right, size / 2);
        zbl = MaxZone(arb->bot_left, size / 2);
        if (ztl >= ztr && ztl >= zbl && ztl >= zbr) {
            max = ztl;
        } else if (ztr >= ztl && ztr >= zbl && ztr >= zbr) {
            max = ztr;
        } else if (zbl >= ztl && zbl >= ztr && zbl >= zbr) {
            max = zbl;
        } else {
            max = zbr;  // the maximum length is returned
        }
        return max;
    }
}

/* Functie care initializeaza o coada vida */
/* Function that initializes an empty queue */
TCoada* InitQ () {
    TCoada* c;
    c = (TCoada*)malloc(sizeof(TCoada));
    if (!c) {
        return NULL;
    }
    c->inc = c->sf = NULL;
    return c;
}

/* Functie de introducere elemente in coada */
/* Function that inserts elements into the queue */
int IntrQ(TCoada *c, TArb x) {
    TLista aux;
    aux = (TLista)malloc(sizeof(TCelula));
    if (!aux) {
        return 0;
    }
    aux->info = x; aux->urm = NULL;
    if (c->sf != NULL) {
        c->sf->urm = aux;
    } else {
        c->inc = aux;
    }
    c->sf = aux;
    return 1;
}

/* Functie de extragere elemente din coada */
/* Function that extracts elements from the queue */
TArb ExtrQ(TCoada *c, TArb *x) {
  TLista aux;
  if (c->inc == NULL) {
    return NULL;
  } else {
    if (c->inc == c->sf) {
      *x = c->inc->info;
      aux = c->inc;
      c->inc = NULL;
      c->sf = NULL;
      free(aux);
    } else {
      *x = c->inc->info;
      aux = c->inc;
      c->inc = c->inc->urm;
      free(aux);
    }
  }
  return *x;
}

/* Functie de distrugere coada */
/* Function that destroys the queue */
void DistrQ(TCoada **c) {
  TLista p, aux;
  p = (*c)->inc;
  while (p) {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(*c);
  *c = NULL;
}

/* Functie care scrie informatii in fisierul (pentru task2) */
/* Function that writes information to the file (for task2) */
void CompressFile(TArb arb, FILE *f) {
    unsigned char node_type = 0;
    TCoada *c = InitQ();
    IntrQ(c, arb);
    while (c->inc) {  // the tree is traversed in BFS order
        TArb aux = ExtrQ(c, &arb);
        if (aux->top_left) {
            // the node is internal, the type becomes 0 and must be written to the file
            node_type = 0;
            fwrite(&node_type, sizeof(unsigned char), 1, f);
        } else if (!aux->top_left && !aux->top_right && !aux->bot_left && !aux->bot_right) {
            // if the node is a leaf, the type becomes 1 and must be written to the file
            node_type = 1;
            fwrite(&node_type, sizeof(unsigned char), 1, f);
            // for leaves, RGB values must be written too
            fwrite(&(aux->red), sizeof(unsigned char), 1, f);
            fwrite(&(aux->green), sizeof(unsigned char), 1, f);
            fwrite(&(aux->blue), sizeof(unsigned char), 1, f);
        }
        if (aux->top_left) {
            IntrQ(c, aux->top_left);
            IntrQ(c, aux->top_right);
            IntrQ(c, aux->bot_right);
            IntrQ(c, aux->bot_left);
        }
    }
    DistrQ(&c);
}

/* Functie care construieste arborele pe baaza fisierului comprimat */
/* Function that builds the tree based on the compressed file */
void ConstrArb(TArb *arb, FILE *f) {
    TCoada *c = InitQ();
    *arb = InitArb();
    IntrQ(c, *arb);  // the queue is initialized with the root
    unsigned char node_type;
    while (fread(&node_type, sizeof(unsigned char), 1, f)) {
        if (node_type == 0) {       // daca tipul citit e 0, initializez 4 copii
            // if the read type is 0, 4 children are initialized
            // and added to the queue
            TArb aux1 = InitArb();
            c->inc->info->top_left = aux1;
            IntrQ(c, aux1);
            TArb aux2 = InitArb();
            c->inc->info->top_right = aux2;
            IntrQ(c, aux2);
            TArb aux3 = InitArb();
            c->inc->info->bot_right = aux3;
            IntrQ(c, aux3);
            TArb aux4 = InitArb();
            c->inc->info->bot_left = aux4;
            IntrQ(c, aux4);
            // the current node is removed from the queue
            TLista aux_lista = c->inc;
            // updating the beginning of the queue
            c->inc = c->inc->urm;
            free(aux_lista);
        } else if (node_type == 1) {
            // if the read type is 1, the RGB values are assigned
            TArb aux = c->inc->info;  // first node is a leaf
            fread(&(aux->red), sizeof(unsigned char), 1, f);
            fread(&(aux->green), sizeof(unsigned char), 1, f);
            fread(&(aux->blue), sizeof(unsigned char), 1, f);
            // the first node is removed from the queue
            TLista aux_lista = c->inc;
            // updating the beginning of the queue
            c->inc = c->inc->urm;
            free(aux_lista);
        }
    }
    DistrQ(&c);
}

/* Functie de decompresie a arborelui rezultat din fisier */
/* Function that decompresses the tree resulting from the file */
void DecompressArb(TArb arb, TPixel ***grid, int x, int y, int size) {
	int i, j;
	if (!arb->top_left && !arb->top_right && !arb->bot_left && !arb->bot_right) {
		for (i = x; i < x + size; i++) {
			for (j = y; j < y + size; j++) {
                // when a leaf is reached, the RGB values are written to the matrix
				(*grid)[i][j].red   = arb->red;
				(*grid)[i][j].green = arb->green;
				(*grid)[i][j].blue  = arb->blue;
			}
        }
	} else {
        DecompressArb(arb->top_left, grid, x, y, size / 2);
		DecompressArb(arb->top_right, grid, x, y + (size / 2), size / 2);
		DecompressArb(arb->bot_right, grid, x + (size / 2), y + (size / 2), size / 2);
        DecompressArb(arb->bot_left, grid, x + (size / 2), y, size / 2);
	}
}
