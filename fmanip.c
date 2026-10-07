#include "fmanip.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EOL "\r\n\0"

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
    char* newLine = malloc(strlen(line) + 1);
    mempcpy(newLine, line, strlen(line) + 1);
    flines_t->linesList[flines_t->nbLines] = newLine;
    flines_t->nbLines++;
    return 0;
}

/* subdivise the string `line` into strings fitting the terminal screen and append them to `flines_t->lines`*/
int parse_line(char* line, int lineLength, struct flines* flines_t)
{
    char* eol = EOL;

    size_t len = strlen(line);
    size_t i = 0;

    while ((len - i) > (size_t)lineLength) {
        char* currentP = line + i;
        char newLine[lineLength + 5];
        mempcpy(newLine, currentP, lineLength);
        mempcpy(newLine + lineLength, eol, 3);
        flines_add_line(flines_t, newLine);
        i += lineLength;
    }
    char* currentP = line + i;
    char newLine[lineLength + 5];
    size_t chunkLen = strlen(currentP);
    if (currentP[chunkLen - 1] == '\n') {
        mempcpy(newLine, currentP, chunkLen - 1);
        mempcpy(newLine + chunkLen - 1, eol, 3);
    } else {
        mempcpy(newLine, currentP, chunkLen);
        mempcpy(newLine + chunkLen, eol, 3);
    }
    flines_add_line(flines_t, newLine);
    return 0;
}

int parse_file(char* fpath, int lineLength, struct flines* flines_t)
{
    FILE* f;

    f = fopen(fpath, "r");
    size_t limit = 1024;
    while (1) {
        char raw_line[limit];
        if (fgets(raw_line, limit, f) == NULL) {
            parse_line(raw_line, lineLength, flines_t);
            break;
        }
        parse_line(raw_line, lineLength, flines_t);
    }
    fclose(f);

    return 0;
}

/*int main(int argc, char** argv)
{
    if (argc <= 1) {
        printf("Missing file argument \n");
        return 1;
    }
    char* fpath = argv[1];
    char* line1 = "première ligne\n deuxième ligne\n";
    struct flines txtfile = init_flines();
    parse_file(fpath, 80, &txtfile);
    for (int i = 0; i < txtfile.nbLines; i++) {
        printf("%s", txtfile.linesList[i]);
    }
    free_flines(&txtfile);
    return 0;
}*/