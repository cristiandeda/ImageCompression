#include "quadtree.h"

int main(int argc, char* argv[]) {
    int i = 0, j = 0;
    int factor;
    char file_in[100], file_out[100];

    // the task is checked
    if (strcmp(argv[1], "-c1") == 0) {
        factor = atoi(argv[2]);
        strcpy(file_in, argv[3]);
        strcpy(file_out, argv[4]);
        FILE *f = fopen(file_in, "rb");

        if (!f) {
            printf("ERROR: File couldn't open!\n");
            return -1;
        }

        char type[3];
        int img_width, img_height, img_max_val;

        fscanf(f, "%s\n%d %d\n%d", type, &img_width, &img_height, &img_max_val);
        fseek(f, 1, SEEK_CUR);

        // the matrix is initialized
        TPixel **img_grid;
        img_grid = (TPixel **)malloc(img_height * sizeof(TPixel*));

        for (i = 0; i < img_height; i++) {
            img_grid[i] = (TPixel *)calloc(img_width, sizeof(TPixel));
        }

        // the matrix information is read
        for (i = 0; i < img_height; i++) {
            for (j = 0; j < img_width; j++) {
                fread(&img_grid[i][j].red, sizeof(unsigned char), 1, f);
                fread(&img_grid[i][j].green, sizeof(unsigned char), 1, f);
                fread(&img_grid[i][j].blue, sizeof(unsigned char), 1, f);
            }
        }

        fclose(f);

        TArb quadtree = NULL;
        CompressArb(&quadtree, img_grid, 0, 0, img_height, factor);
        int levels = NrNiv(quadtree);
        int leaves_nr = NrLeaves(quadtree);
        int max_zone = MaxZone(quadtree, img_height);
        FILE *fout = fopen(file_out, "wt");

        if (!fout) {
            printf("ERROR: File_Out couldn't open!\n");
            return -1;
        }

        fprintf(fout, "%d\n", levels);
        fprintf(fout, "%d\n", leaves_nr);
        fprintf(fout, "%d\n", max_zone);
        fclose(fout);

        // memory is released
        for (i = 0; i < img_height; i++) {
            free(img_grid[i]);
        }
        free(img_grid);
        DistrArb(&quadtree);

    } else if (strcmp(argv[1], "-c2") == 0) {
        factor = atoi(argv[2]);
        strcpy(file_in, argv[3]);
        strcpy(file_out, argv[4]);
        FILE *f = fopen(file_in, "rb");

        if (!f) {
            printf("ERROR: File couldn't open!\n");
            return -1;
        }

        char type[3];
        unsigned int img_width, img_height, img_max_val;

        fscanf(f, "%s\n%d %d\n%d", type, &img_width, &img_height, &img_max_val);
        fseek(f, 1, SEEK_CUR);

        TPixel **img_grid;
        img_grid = (TPixel **)malloc(img_height * sizeof(TPixel*));

        for (i = 0; i < img_height; i++) {
            img_grid[i] = (TPixel *)calloc(img_width, sizeof(TPixel));
        }

        // the matrix information is read
        for (i = 0; i < img_height; i++) {
            for (j = 0; j < img_width; j++) {
                fread(&img_grid[i][j].red, sizeof(unsigned char), 1, f);
                fread(&img_grid[i][j].green, sizeof(unsigned char), 1, f);
                fread(&img_grid[i][j].blue, sizeof(unsigned char), 1, f);
            }
        }

        fclose(f);

        TArb quadtree = NULL;
        CompressArb(&quadtree, img_grid, 0, 0, img_height, factor);
        FILE *fout = fopen(file_out, "wb");

        if (!fout) {
            printf("ERROR: File_Out couldn't open!\n");
            return -1;
        }

        fwrite(&img_height, sizeof(unsigned int), 1, fout);
        CompressFile(quadtree, fout);
        fclose(fout);

        // memory is released
        for (i = 0; i < img_height; i++) {
            free(img_grid[i]);
        }
        free(img_grid);
        DistrArb(&quadtree);

    } else if (strcmp(argv[1], "-d") == 0) {
        strcpy(file_in, argv[2]);
        strcpy(file_out, argv[3]);
        FILE *f = fopen(file_in, "rb");

        if (!f) {
            printf("ERROR: File couldn't open!\n");
            return -1;
        }

        unsigned int size;

        fread(&size, sizeof(unsigned int), 1, f);
        TPixel **img_grid;
        img_grid = (TPixel **)malloc(size * sizeof(TPixel*));

        for (i = 0; i < size; i++) {
            img_grid[i] = (TPixel *)calloc(size, sizeof(TPixel));
        }

        TArb quadtree = NULL;
        ConstrArb(&quadtree, f);
        fclose(f);
        DecompressArb(quadtree, &img_grid, 0, 0, size);
        FILE *fout = fopen(file_out, "wb");

        if (!fout) {
            printf("ERROR: File_Out couldn't open!\n");
            return -1;
        }

        // the matrix information is written to the file
        fprintf(fout, "P6\n");
        fprintf(fout, "%d %d\n", size, size);
        fprintf(fout, "255\n");

        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                fwrite(&img_grid[i][j].red, sizeof(unsigned char), 1, fout);
                fwrite(&img_grid[i][j].green, sizeof(unsigned char), 1, fout);
                fwrite(&img_grid[i][j].blue, sizeof(unsigned char), 1, fout);
            }
        }

        fclose(fout);
        
        // memory is released
        for (i = 0; i < size; i++) {
            free(img_grid[i]);
        }
        free(img_grid);
        DistrArb(&quadtree);
    }
    return 0;
}