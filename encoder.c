/*
 * N-queen problem SAT encoder
 * Author Daniel Brashaw
**/

#define FILE_NAME       "queen%d.cnf"
#define MINISAT_FILE    "queen%d.out"
#define BUFFER_SZ       80

typedef unsigned long long u64;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void queen_gen(int ndim) {
    u64 nclauses = 0;
    char fname[BUFFER_SZ+1];
    fname[BUFFER_SZ] = '\0';

    snprintf(fname, BUFFER_SZ, FILE_NAME, ndim);
    FILE *fp = fopen(fname, "w");
  
    // hacky add empty bytes so that first line does not get overwritten
    for (int i = 0; i < ndim; i++) {
        fprintf(fp, "   ");
    }
    fputc('\n', fp);

    for (int x = 1; x <= ndim*ndim; x++) {
        fprintf(fp, "%d ", x);
        if (x % ndim == 0) {
            fprintf(fp, "0\n");
            nclauses++;
        }
    }
    // not in a row
    for (u64 r = 1; r <= ndim*(ndim-1)+1; r+=ndim) {
        for (u64 i = r; i < r+ndim-1; i++) {
            for (u64 j = i+1; j < r+ndim; j++) {
                fprintf(fp, "%llu %llu 0\n", -i, -j);
                nclauses++;
            }
        }
    }
    // not in a column
    for (u64 c = 1; c <= ndim; c++) {
        for (u64 i = c; i <= ndim*(ndim-1); i+=ndim) {
            for (u64 j = i+ndim; j <= ndim*ndim; j+=ndim) {
                fprintf(fp, "%llu %llu 0\n", -i, -j);
                nclauses++;
            }
        }
    }
    // not on antidiagonal
    for (u64 r = 0; r < ndim-1; r++) {
        bool wrap = false;
        for (u64 c = 0; c < ndim-1; c++) {
            u64 i = r*ndim+c+1;
            for (u64 j = i+ndim+1; j<=ndim*ndim && !wrap; j+=ndim+1) {
                if (!j % ndim) wrap = true;
                else {
                    fprintf(fp, "%llu %llu 0\n", -i, -j);
                    nclauses++;
                }
            }
        }
    }
    // not on diagonal
    for (u64 r = ndim; r <= ndim*(ndim-1); r+=ndim) {
        for (u64 c = 0; c < ndim-1; c++) {
            u64 i = r-c;
            bool wrap = false;
            for (u64 j = i+ndim-1; j<=ndim*ndim && !wrap; j+=ndim-1) {
                fprintf(fp, "%llu %llu 0\n", -i, -j);
                nclauses++;
                if (!((j-1) % ndim)) wrap = true;
            }
        }
    }
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "p cnf %d %llu", ndim*ndim, nclauses);
    fclose(fp);
}

int main() {
    int nqueens[] = { 7, 8, 9, 10, 15, 20, 25 };
    for (int i = 0; i < 7; i++) {
        queen_gen(nqueens[i]);
    }
    return 0;
}