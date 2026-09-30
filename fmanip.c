#include "fmanip.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct flines init_flines()
{
    struct flines new_flines;
    new_flines.nbLines = 0;
    new_flines.maxLines = 0;
    new_flines.linesList = NULL;
    return new_flines;
}

int free_flines(struct flines* flines_t)
{
    if (flines_t->linesList != NULL) {
        for (int i = 0; i < flines_t->maxLines; i++) {
            free(flines_t->linesList[i]);
        }
        free(flines_t->linesList);
        flines_t->linesList = NULL;
        return 0;
    }
    return 1;
}

int flines_add_line(struct flines* flines_t, char* line)
{
    if (flines_t->nbLines == flines_t->maxLines) {
        size_t newSize = (flines_t->maxLines * 2 + 1) * sizeof(char*);
        flines_t->linesList = realloc(flines_t->linesList, newSize);
        if (flines_t->linesList == NULL) {
            perror("realloc() flines_add_line() failed");
            return 1;
        }
        flines_t->maxLines = flines_t->maxLines * 2 + 1;
    }

    flines_t->linesList[flines_t->nbLines] = line;
    flines_t->nbLines++;
    return 0;
}

struct flines parse_file(char* fpath, int lineLength)
{
    FILE* f;

    f = fopen(fpath, "r");
    struct flines flines = init_flines();

    while (1) {
        char* line;
        line = malloc(lineLength + 1);
        if (fgets(line, lineLength, f) == NULL) {
            break;
        }
        flines_add_line(&flines, line);
    }
    fclose(f);

    return flines;
}