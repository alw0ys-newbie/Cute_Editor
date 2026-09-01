#ifndef DATA
#define DATA

#include <stdlib.h>
#include <termios.h>

/*** editorConfig ***/

/* Describes the actual configuration of the editor*/
typedef struct editorConfig {
    int rows;
    int cols;
    struct termios orig_termios;
} editorConfig;

/*** textBuffer***/

typedef struct textBuffer* textBuffer_t;

/* Initiates a textBuffer instance and returns a pointer to it*/
textBuffer_t initBuffer();

/* Append `string` to the text buffer pointed at by `tB`*/
void appendBuffer(textBuffer_t tB, char* string);

/* Free the memory allocated to the textBuffer instance `tB`*/
void destroyBuffer(textBuffer_t tB);
char* getBufferString(textBuffer_t tB);
size_t getBufferLen(textBuffer_t tB);

#endif