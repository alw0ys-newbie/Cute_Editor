#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** Text Buffer definition ***/

/* struct containing a string and its length*/
struct textBuffer {
    size_t len;
    char* string;
};

textBuffer_t initBuffer()
{
    textBuffer_t buffer = malloc(sizeof(struct textBuffer));
    buffer->string = (char*)malloc(1);

    if (buffer->string == NULL) {
        perror("malloc buffer string failed");
        exit(EXIT_SUCCESS);
    }
    buffer->len = 0;
    return buffer;
}

void destroyBuffer(textBuffer_t tB)
{
    free(tB->string);
    tB->string = NULL;
    free(tB);
    tB = NULL;
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
    size_t lenNew = strlen(new);
    tB->len += lenNew;
    tB->string = realloc(tB->string, tB->len);

    if (tB->string == NULL) {
        perror("malloc tB string failed");
        exit(EXIT_SUCCESS);
    }

    strcat(tB->string, new);
}

/*int main()
{
    textBuffer_t buff = initBuffer();
    printf("len : %zu , string : %s\n", buff->len, buff->string);
    appendBuffer(buff, "bonjour !");
    printf("%zu\n", strlen("bonjour !"));
    printf("len : %zu , string : %s\n", getBufferLen(buff), getBufferString(buff));
    destroyBuffer(buff);
}*/