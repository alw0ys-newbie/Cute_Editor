#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/*** Text Buffer definition ***/

struct textBuffer {
    size_t len;
    char* string;
};

textBuffer_t initBuffer()
{
    textBuffer_t buffer = malloc(sizeof(struct textBuffer));
    if (buffer == NULL) {
        perror("malloc init_buffer() failed");
        exit(EXIT_FAILURE);
    }
    buffer->string = NULL;
    buffer->len = 0;
    return buffer;
}

void destroyBuffer(textBuffer_t tB)
{
    free(tB->string);
    free(tB);
}

char* getBufferString(textBuffer_t tB)
{
    return tB->string;
}

size_t getBufferLen(textBuffer_t tB)
{
    return tB->len;
}

void appendBuffer(textBuffer_t tB, char* string)
{
    char* new = string;
    size_t sizeNew = strlen(new) + 1;

    tB->string = realloc(tB->string, tB->len + sizeNew);

    if (tB->string == NULL) {
        perror("realloc() append_buffer() failed");
        exit(EXIT_FAILURE);
    }
    void* dest = &(tB->string[tB->len]);
    memcpy(dest, new, sizeNew);
    tB->len += sizeNew - 1;
}
