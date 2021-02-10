/*
 * N-queen problem SAT decoder
 * Author Daniel Brashaw
**/

// the buffer size must be large enough to hold the entire CNF
// result of running minisat

// run with "./decoder <minisat output>"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ       4096

int main(int argc, char *argv[]) {

    FILE *fp = NULL;
    if (argc == 1) {
        fprintf(stderr, "usage: decoder <filename>\n");
        exit(-1);
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "error opening %s\n", argv[1]);
        }
    }
    else {
        fprintf(stderr, "unknown usage error\n");
        exit(-1);
    }

    char buffer[BUFFER_SZ], bufcopy[BUFFER_SZ];
    memset(buffer, 0, BUFFER_SZ);
    buffer[BUFFER_SZ-1] = '\0';

    while (fgets(buffer, BUFFER_SZ, fp) != NULL) {
        if (!strncmp("SAT", buffer, 3)) {
            puts("Satisfied\n");
        }
        else if (!strncmp("UNSAT", buffer, 5)){
            puts("Unsatisfied\n");
            fclose(fp);
            exit(1);
        }
        else {
            strcpy(bufcopy, buffer);
            char *token = strtok(bufcopy, " ");
            int pos_literals = 0, v, dim, c = 0;
            while (token != NULL) {
                v = atoi(token);
                if (v > 0)
                    pos_literals++;
                token = strtok(NULL, " ");
            }
            dim = pos_literals;
            token = strtok(buffer, " ");
            while (token != NULL) {
                c++;
                v = atoi(token);
                if (v > 0) 
                    printf("Q ");
                else if (v < 0)
                    printf(". ");
                if (c % dim == 0)
                    printf("\n");
                
                token = strtok(NULL, " ");
            }
        }
    }
    printf("\n");
    fclose(fp);
}